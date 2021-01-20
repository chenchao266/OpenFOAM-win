#include "blockDescriptor.h"
namespace Foam
{
    inline point faceCorr(const point& p)
    {
        return p/mag(p);
    }
}

 