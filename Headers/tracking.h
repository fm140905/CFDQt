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

#include <random>
#include <iostream>
#include "cell.h"

bool deltaTracking(Particle& particle, const MCSettings& config);
bool deltaTrackingNeutron(Particle& particle, const MCSettings& config);
int ComptonScattering(Particle& particle, const MCSettings& config);
int neutronElasticScattering(Particle& particle, const MCSettings& config);

// int fastNeutronElasticScatterSampling(const double A, double& E_lab, double& mu_lab);
int thermalNeutronElasticScatterSampling(const double A, const double E_0, double& E_lab, double& mu_lab);

class NormalRandNumGenerator
{
private:
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
public:
    NormalRandNumGenerator(double mean, double stddev)
        : distribution(mean, stddev)
    {}
    NormalRandNumGenerator(double mean, double stddev, int seed)
        : generator(seed), distribution(mean, stddev) 
    {}
    double genRandNumber() {return distribution(generator);}
};
#endif // TRACKING_H