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
    alpha = 2 * M_PI * QRandomGenerator::global()->generateDouble(); // angel phi
    R1 = particle.dir.x();
    R2 = particle.dir.y();
    R3 = particle.dir.z();
    double sinAng = std::sqrt(1 - cosAng * cosAng);
    if (std::abs(std::abs(R3) - 1) > 1e-8)
    {
        eta = 1.0 / std::sqrt(1 - R3 * R3);
        particle.dir.setX(cosAng * R1 + sinAng * (std::cos(alpha) * R3 * R1 - std::sin(alpha) * R2) * eta);
        particle.dir.setY(cosAng * R2 + sinAng * (std::cos(alpha) * R3 * R2 + std::sin(alpha) * R1) * eta);
        particle.dir.setZ(cosAng * R3 - sinAng * std::cos(alpha) / eta);
    }
    else
    {
        particle.dir.setX(sinAng * std::cos(alpha));
        particle.dir.setY(sinAng * std::sin(alpha));
        particle.dir.setZ(cosAng * R3);
    }
    return 0;
}