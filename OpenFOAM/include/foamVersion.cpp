#include "foamVersion.H"
 
///todo...
 
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
