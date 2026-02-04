#include "Guid.h"
#include "Platform/PlatformMisc.h"

namespace Sun
{
  
    bool operator == (const Guid& g1, const Guid& g2) {
        return (g1.A == g2.A) && (g1.B == g2.B) && (g1.C == g2.C) && (g1.D == g2.D);
    }

    bool operator != (const Guid& g1, const Guid& g2) {
        return !(g1 == g2);
    }

}

