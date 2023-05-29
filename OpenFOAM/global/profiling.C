/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2009-2016 Bernhard Gschaider
    Copyright (C) 2016-2020 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "profiling.H"
#include "profilingInformation.H"
#include "profilingSysInfo.H"
#include "cpuInfo.H"
#include "memInfo.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
int profiling::allowed(debug::infoSwitch("allowProfiling", 1));
std::unique_ptr<profiling> profiling::singleton_(nullptr);


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

profilingInformation* profiling::create()
{
    // Top-level entry: reset everything
    pool_.clear();
    children_.clear();
    stack_.clear();
    times_.clear();

    Information* info = new Information;

    pool_.append(info);
    children_.resize(pool_.size());
    children_.last().clear();  // safety

    return info;
}


profilingInformation* profiling::create
(
    profilingInformation *parent,
    const string& descr
)
{
    const label parentId = parent->id();

    for (Information* child : children_[parentId])
    {
        if (descr == child->description())
        {
            return child;  // Found existing
        }
    }

    Information* info = new Information(parent, descr, pool_.size());

    pool_.append(info);
    children_.resize(pool_.size());
    children_.last().clear();  // safety
    children_[parentId].append(info);

    return info;
}


void profiling::beginTimer(profilingInformation *info)
{
    stack_.append(info);
    times_.append(clockValue::now());
    info->setActive(true);              // Mark as on stack
}


profilingInformation* profiling::endTimer()
{
    Information *info = stack_.remove();
    clockValue clockval = times_.remove();

    info->update(clockval.elapsed());   // Update elapsed time
    info->setActive(false);             // Mark as off stack

    return info;
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

bool profiling::active()
{
    return allowed && singleton_;
}


void profiling::disable()
{
    allowed = 0;
}


bool profiling::print(Ostream& os)
{
    if (active())
    {
        return singleton_->writeData(os);
    }

    return false;
}


bool profiling::writeNow()
{
    if (active())
    {
        return singleton_->regIOobject::write();
    }

    return false;
}


void profiling::initialize
(
    const IOobject& ioObj,
    const Time& owner
)
{
    if (allowed && !singleton_)
    {
        singleton_.reset(new profiling(ioObj, owner));
    }
}


void profiling::initialize
(
    const dictionary& dict,
    const IOobject& ioObj,
    const Time& owner
)
{
    if (allowed && !singleton_)
    {
        singleton_.reset(new profiling(dict, ioObj, owner));
    }
}


void profiling::stop(const Time& owner)
{
    if (singleton_ && &owner == &(singleton_->owner_))
    {
        singleton_.reset(nullptr);
    }
}


profilingInformation* profiling::New(const string& descr)
{
    Information *info = nullptr;

    if (active())
    {
        Information *parent = singleton_->stack_.last();

        info = singleton_->create(parent, descr);
        singleton_->beginTimer(info);

        if (singleton_->memInfo_)
        {
            info->maxMem_ = max
            (
                info->maxMem_,
                singleton_->memInfo_->update().size()
            );
        }
    }

    return info;
}


void profiling::unstack(const profilingInformation *info)
{
    if (active() && info)
    {
        Information *top = singleton_->endTimer();

        if (info->id() != top->id())
        {
            FatalErrorInFunction
                << "Profiling information to unstack has different id than"
                << " the top of the profiling stack" << nl
                << "  info: " << info->id() << " (" << info->description()
                << ")\n"
                << "  top:  " << top->id()  << " (" << top->description()
                << ")\n" << endl
                << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

profiling::profiling
(
    const IOobject& io,
    const Time& owner,
    const bool allEnabled
)
:
    IOdictionary(io),
    owner_(owner),
    pool_(),
    children_(),
    stack_(),
    times_(),
    sysInfo_(nullptr),
    cpuInfo_(nullptr),
    memInfo_(nullptr)
{
    if (allEnabled)
    {
        sysInfo_.reset(new profilingSysInfo);
        cpuInfo_.reset(new cpuInfo);
        memInfo_.reset(new memInfo);
    }

    Information *info = this->create();
    this->beginTimer(info);

    DetailInfo << "profiling initialized" << nl;
}


profiling::profiling
(
    const dictionary& dict,
    const IOobject& io,
    const Time& owner
)
:
    profiling(io, owner, false)
{
    if (dict.getOrDefault("sysInfo", false))
    {
        sysInfo_.reset(new profilingSysInfo);
    }
    if (dict.getOrDefault("cpuInfo", false))
    {
        cpuInfo_.reset(new cpuInfo);
    }
    if (dict.getOrDefault("memInfo", false))
    {
        memInfo_.reset(new memInfo);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

profiling::~profiling()
{
    if (this == singleton_.get())
    {
        singleton_.reset(nullptr);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Time& profiling::owner() const
{
    return owner_;
}


label profiling::size() const noexcept
{
    return stack_.size();
}


bool profiling::writeData(Ostream& os) const
{
    static DynamicList<scalar> elapsed;

    const clockValue now(clockValue::now());

    const label nstack = stack_.size();

    elapsed.resize(nstack+1);   // extend for last entry, which has no child.

    for (label stacki=0; stacki < nstack; ++stacki)
    {
        elapsed[stacki] = (now - times_[stacki]);
    }
    elapsed.last() = 0;

    os.beginBlock("profiling");

    // Active items
    for (label stacki=0; stacki < nstack; ++stacki)
    {
        if (stacki) os << nl;   // Extra line between entries

        stack_[stacki]->write
        (
            os,
            true,
            elapsed[stacki],    // elapsedTime
            elapsed[stacki+1]   // childTimes
        );
    }

    // Non-active items
    for (const Information& info : pool_)
    {
        if (!info.active())
        {
            os << nl;
            info.write(os);
        }
    }

    os.endBlock();

    if (sysInfo_)
    {
        os << nl;
        os.beginBlock("sysInfo");
        sysInfo_->write(os);
        os.endBlock();
    }

    if (cpuInfo_)
    {
        os << nl;
        os.beginBlock("cpuInfo");
        cpuInfo_->write(os);
        os.endBlock();
    }

    if (memInfo_)
    {
        memInfo_->update();

        os << nl;
        os.beginBlock("memInfo");
        memInfo_->write(os);
        os.writeEntry("units", "kB");
        os.endBlock();
    }

    return os.good();
}


bool profiling::writeObject
(
    IOstreamOption,
    const bool valid
) const
{
    return regIOobject::writeObject(IOstreamOption(IOstream::ASCII), true);
}


// ************************************************************************* //

 } // End namespace Foam
