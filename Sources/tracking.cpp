#include "tracking.h"

bool deltaTracking(Particle& particle, const MCSettings& config)
{
    const double muMax = config.getMuMax(particle.ergE);
    while (!particle.escaped)
    {
        // randomly select a distance
        double randReal = QRandomGenerator::global()->generateDouble();
        double distance = - std::log(randReal) / muMax; // cm
        particle.move(distance);
        if(!config.ROI.contain(particle.pos))
        {
            // escaped
            particle.escaped = true;
            return false;
        }

        // virtual collision
        // check if randReal < u(x,E) / u_max
        randReal = QRandomGenerator::global()->generateDouble();
        // const Cell& currentCell = config.getCell(particle);
        const Cell& currentCell = config.cells[0];
        if (randReal * currentCell.material.getPhotonTotalAtten(particle.ergE) < muMax)
        {
            // update the wieght, w = w * P(interaction is Compton scattering)
            particle.weight *= currentCell.material.getPhotonCrossSection().getComptonOverTotal(particle.ergE);
            return true;
            // Compton scattering happens next
        }
    }
    return false;
}

int ComptonScattering(Particle& particle, const MCSettings& config)
{
    // Kahn's rejection algorithm
    // randomly choose an angle based on the K-N equation
    // find the new energy and angle after Compton scatter
    double R1, R2, R3;
    double alpha = particle.ergE / 0.511; // incoming photon energy in electron rest mass units
    double eta=0;
    double cosAng = 0;
    while (1)
    {
        R1 = QRandomGenerator::global()->generateDouble();
        R2 = QRandomGenerator::global()->generateDouble();
        R3 = QRandomGenerator::global()->generateDouble();
        if((2*alpha+9)* R1 <= (2*alpha + 1))
        {
            eta = 1 + 2 * alpha * R2;
            if (R3*eta*eta <= 4*(eta-1))
            {
                cosAng = 1 - 2 * R2;
                break;
            }
            else
            {
                continue;
            }
            
        }
        else
        {
            eta = (2*alpha+1)/(2*R2*alpha+1);
            cosAng = 1-(eta-1)/alpha;
            if(R3 <= 0.5*(cosAng * cosAng + 1/ eta))
            {
                break;
            }
            else
            {
                continue;
            }
        }
    }

    // update particle energy
    particle.ergE /= eta; // energy of scattered photon
    // update particle direction
    particle.scatter(cosAng);
    return 0;
}


bool deltaTrackingNeutron(Particle& particle, const MCSettings& config)
{
    // only one material
    const double muMax = config.cells[0].material.getNeutronTotalAtten(particle.ergE); // cm^-1
    while (!particle.escaped)
    {
        // randomly select a distance
        double randReal = QRandomGenerator::global()->generateDouble();
        double distance = - std::log(randReal) / muMax; // cm
        particle.move(distance);
        if(!config.ROI.contain(particle.pos))
        {
            // escaped
            particle.escaped = true;
            return false;
        }

        // virtual collision
        // check if randReal < u(x,E) / u_max, which is always true for one material
        return true;
        // Neutron elastic scattering happens next
    }
    return false;
}

int neutronElasticScattering(Particle& particle, const MCSettings& config)
{
    const Cell& currentCell = config.cells[0];
    // decide which nuclide the neutron will interacts with
    double randReal = QRandomGenerator::global()->generateDouble();
    const Nuclide& nuclide = currentCell.material.selectInteractionTarget(particle.ergE, randReal);
    double A = nuclide.getAtomicWeight();
    // update the wieght, w = w * P(interaction is Elastic scattering)
    particle.weight *= nuclide.getNeutronCrossSection().getElasticMicroScopicCrossSectionAt(particle.ergE) 
                        / nuclide.getNeutronCrossSection().getTotalMicroScopicCrossSectionAt(particle.ergE);
    double E_lab;
    double mu_lab;
    // if nuclide is H-1, simple case
    if (std::abs(A-1) < 0.1)
    {
        // isotopic in CMS
        double mu_cms = 2 * QRandomGenerator::global()->generateDouble() - 1; // -1 < mu_cms < 1
        mu_lab = std::sqrt((1+mu_cms) / 2);
        E_lab = (1+mu_cms) / 2;
    }
    else
    {
        // sample a mu in CMS
        double randReal = QRandomGenerator::global()->generateDouble();
        double mu_cms = nuclide.getNeutronCrossSection().getDAInvCDFAt(particle.ergE, randReal);
        double E_cms = std::pow(A/(1+A), 2);
        // calculate the energy in lab system
        E_lab = (1+A*A+2*A*mu_cms) / ((1+A) * (1+A));
        // convert to lab system
        mu_lab = mu_cms * std::sqrt(E_cms / E_lab) + 1 / (A+1) * std::sqrt(1/E_lab);
    }
    // update particle energy
    particle.ergE *= E_lab;
    // update particle's moving direction
    particle.scatter(mu_lab);

    return 0;
}