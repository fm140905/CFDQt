/**
 * @file tracking.h
 * @brief photon transport using delta-tracking algorithm
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef TRACKING_H
#define TRACKING_H

#include "cell.h"

class MCSettings
{
private:
    const Cylinder& cylinder;
    const Source& source;
    const int maxN;
    const int maxScatterN;
    const double minW;
    const double minE;
public:
    MCSettings(const Cylinder& cyl, const Source& src, const int maxn, 
               const int maxscattern, const double minw, const double mine)
        : cylinder(cyl), source(src), maxN(maxn), maxScatterN(maxscattern),
          minW(minw), minE(mine)
    {}
    // ~MCSettings();
};

int deltaTracking(Particle& particle, const MCSettings& config);
int ComptonScattering(Particle& particle, const MCSettings& config);

#endif // TRACKING_H