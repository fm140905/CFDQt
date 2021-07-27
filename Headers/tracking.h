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

int deltaTracking(Particle& particle, const MCSettings& config);
int ComptonScattering(Particle& particle, const MCSettings& config);

#endif // TRACKING_H