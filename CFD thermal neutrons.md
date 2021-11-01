<!--
 * @Description: 
 * @Author: Ming Fang
 * @Date: 1969-12-31 18:00:00
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-11-01 18:22:13
-->
# CFD
The CFD algorithm consists of two parts, one responsible for tracking of particles in the materials, the other responsible for calculating the contributions of scttered particle to the detector volume. The structure is as following:
1. Create a particle from the source
2. *Perform CFD of the primary particle*
3. Move the particle to a new position where a scattering interaction is going to happen using the delta-tracking algorithm. Check if the particle is still within the ROI and if its energy/weight is above the rejection threshold. If not, go to step 1.
4. *Perform CFD of the scattered particle*
5. Update the energy and moving direction of the particle by sampling them from the differential cross-section. Go to step 3.

CFD calculations are performed at step 2 and step 4, where a copy of the particle is sent to the CFD subroutine, without interfereing the tracking of the particle.

In the next sections, we first describe the particle tracking methods for gamma-rays and neutrons (both fast and slow), and then we describe the CFD calculation algorithms.


## Particle Tracking

### Delta-tracking algorithm
Delta-tracking algorithm is used to sample the distance that the particle will travel alone its current moving direction before the next interaction happens. Let $u_{max}(E)$ be the maximum of the total macroscopic cross-sections of all materials, where $E$ is the particle's energy. The distance is sampled as follows:

1. Let the particle move by a distance $d = -\ln(R)/u_{max}(E)$, where $R$ is random number sampled from $U(0,1)$.
2. If the particle is not in ROI, exit.
3. Let $u_i(E)$ be the attenuation coefficient of the material where the particle is now at. If $R < u_i(E) / u_{max}(E)$, exit; else, go back to step 1. 

### Scattering

After delta-tracking, the particle is now going to interact with the material at current position. We first force this interaction to be scattering, by multiplying its weight by a factor
$$
\frac{\sigma_s}{\sigma_{tot}}
$$
where for gamma-rays $\sigma_s$ is tje Compton scattering cross-section, and for neutrons $\sigma_s$ is the elastic scattering  cross-section, and $\sigma_{tot}$ is the total cross-section. The we sample a new energy and a scattering angle from the differential cross-section. Three types of particles are discussed below.

#### Gamma-rays

#### Fast neutrons

#### Thermal neutrons

# CFD of Gamma-rays



# CFD of Neutrons

## Free Gas Treatment
Assuming a Maxwellian energy distribution of the scattering medium, the differential scattering cross section of the scattering atoms in the laboratory system can be approximated as follows:
$$
\frac{d^2\sigma_s}{dEd\mu} = \sigma_0 f(\mu,E)
$$
$$
f(\mu, E) = 2\sqrt{\frac{E}{E_0}}\sqrt{\frac{M}{2\pi kT\epsilon^2}}\exp\left[-\frac{M}{2kT\epsilon^2} (E_0-E-\frac{\epsilon^2}{2M})^2\right]
$$
$$
\epsilon^2 = 2m(E+E_0 -2\mu \sqrt{E_0E})
$$
where $\sigma_0$ is the zero-temperature elastic differential cross section from ENDF, $E_0$ is the neutron energy before scattering, $E$ is the neutron energy after scattering, $M$ is the mass of the scattering nucleus, $m$ is the mass of the neutron, $T$ is the temperature, $k$ is the Boltzmann constant. 

The total elastic scattering crosssection $\sigma_s$ is 
$$
\sigma_s = \int\int \frac{d^2\sigma_s}{dEd\mu} dEd\mu = \sigma_0 F(E_0)
$$
$$
F(E_0) = (1+\frac{1}{2a^2}) \mathrm{erf}(a) +\frac{\exp(-a^2)}{\sqrt{\pi}a}
$$
$$
a = \sqrt{\frac{ME_0}{kT}}
$$
which suggests that we need to raise the zero-temperature elastic cross-section by the factor $F(E_0)$, and increase the total cross-section by the same amount.

## Track Thermal Neutrons
When elastic scattering happens between a thermal neutron and a target nucleus, we need to sample the energy $E$ and scattering angle $\mu$ of the neutron after the scattering based on the differential cross-section. Such a sampling scheme is given in xxx.

## CFD of Thermal Neutrons
The thermal neutron is forced to be scattered towards the dtector, and we can calculate the scattering angle based on the positions of the neutron and the detector. Given $\mu$, the probability density function of $E$:
$$
p(E) = \frac{f(\mu,E)}{F(E_0)}
$$
Assume there are $N$ energy bins in the thermal range, the contribution to each thermal energy bin is
$$
p(E_i)\delta E_i \times \exp\left(-\int u(E_i)dl\right) \times D(\mu, E_i) 
$$
where $E_i$ is the center of $i$-th energy bin, $\delta E_i$ is the width of $i$-th energy bin, $u(E_i)$ is the attenuation coefficient, $D(\mu, E_i)$ is the detector response integrated over solid angle extended by the entire detector volume.