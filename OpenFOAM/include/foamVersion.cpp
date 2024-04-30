#include "foamVersion.H"
 
///todo...
namespace Foam
{
    //- Version information
    namespace foamVersion
    {
        //- OpenFOAM api number (integer) corresponding to the value of OPENFOAM
        //- at the time of compilation.
        //  The value is 0 if OPENFOAM was not defined.
        const int api = 2106;

        //- OpenFOAM patch number as a std::string
        const std::string patch = "";

        //- OpenFOAM build information as a std::string
        const std::string build = "";

        //- OpenFOAM build architecture information
        //- (machine endian, label/scalar sizes) as a std::string
        const std::string buildArch = "";

        //- OpenFOAM version (name or stringified number) as a std::string
        const std::string version = "";
    }
}
bool Foam::foamVersion::patched()
{
    return false;
}

unsigned Foam::foamVersion::labelByteSize(const std::string& str)
{
    return 0;
}

unsigned Foam::foamVersion::scalarByteSize(const std::string& str)
{
    return 0;
}

void Foam::foamVersion::printBuildInfo(std::ostream& os, const bool full)
{
}

std::string Foam::foamVersion::configuredProjectDir()
{
    return std::string();
}

std::string Foam::foamVersion::configuredEtcDir()
{
    return std::string();
}
