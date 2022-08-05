/**
 * @file tracking.h
 * @brief photon transport using delta-tracking algorithm
 * @version 0.1
 * @date 2021-07-26
 * 
 * @author Ming Fang
 * 
 */

#ifndef TRACKING_H
#define TRACKING_H

#include <iostream>
#include "cell.h"

/**
 * @brief Perform delta tracking of a photon. Photon travels along the current direction, 
 *        and its position and weight are updated, 
 *        until a Compton scattering is going to happen.
 * 
 * @param particle Particle to be updated.
 * @param config MC run settings 
 * @return true if photon stays within ROI
 * @return false if photon leaves ROI
 */
bool deltaTracking(Particle& particle, const MCSettings& config);

/**
 * @brief Perform delta tracking of a neutron. Photon travels along the current direction, 
 *        and its position and weight are updated, 
 *        until a Compton scattering is going to happen.
 * 
 * @param particle Photon to be updated.
 * @param config MC run settings 
 * @return true if neutron stays within ROI
 * @return false if neutron leaves ROI
 */
bool deltaTrackingNeutron(Particle& particle, const MCSettings& config);

/**
 * @brief Perform Compton scattering of a photon. 
 *        The scattering angle is sampled based K-N equation.
 *        The photon's energy and moving direction are updated.
 * 
 * @param particle Photon to be scattered.
 * @param config MC run settings
 * @return int
 */
int ComptonScattering(Particle& particle, const MCSettings& config);

/**
 * @brief Perform elastice scattering of a fast/thermal neutron. 
 *        Neutron's energy and moving direction are updated.
 * 
 * @param particle Neutron to be scattered.
 * @param config 
 * @return int 
 */
int neutronElasticScattering(Particle& particle, const MCSettings& config);

// int fastNeutronElasticScatterSampling(const double A, double& E_lab, double& mu_lab);

/**
 * @brief Samples scattering angle and neutron energy in a thermal neutron elastic scattering reaction.
 * 
 * @param A Atomic mass of the nuclide that the neutron interacts with
 * @param E_0 Neutron energy before scattering
 * @param E_lab Neutron energy after scattering / energy before scattering
 * @param mu_lab Cosine of neutron scattering angle in laboratory system
 * @return int 
 */
int thermalNeutronElasticScatterSampling(const double A, const double E_0, double& E_lab, double& mu_lab);

#endif // TRACKING_H