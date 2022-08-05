#include "tracking.h"

bool deltaTracking(Particle& particle, const MCSettings& config)
{
    const double muMax = config.getMuMax(particle.ergE);
    while (!particle.escaped)
    {
        // randomly select a distance
        double randReal = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
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
        randReal = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
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
        R1 = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
        R2 = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
        R3 = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
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
        double randReal = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
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
    double randReal = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
    const Nuclide& nuclide = currentCell.material.selectInteractionTarget(particle.ergE, randReal);
    double A = nuclide.getAtomicWeight();
    // update the wieght, w = w * P(interaction is Elastic scattering)
    particle.weight *= nuclide.getNeutronCrossSection().getElasticMicroscopicCrossSectionAt(particle.ergE) 
                        / nuclide.getNeutronCrossSection().getTotalMicroscopicCrossSectionAt(particle.ergE);
    double E_lab;
    double mu_lab;
    if (particle.ergE > 1) // threshold =  1eV
    {
        // fast
        // if nuclide is H-1, simple case
        if (std::abs(A-1) < 0.1)
        {
            // isotopic in CMS
            double mu_cms = 2 * GlobalUniformRandNumGenerator::GetInstance().generateDouble() - 1; // -1 < mu_cms < 1
            mu_lab = std::sqrt((1+mu_cms) / 2);
            E_lab = (1+mu_cms) / 2;
        }
        else
        {
            // sample a mu in CMS
            double randReal = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
            double mu_cms = nuclide.getNeutronCrossSection().getDAInvCDFAt(particle.ergE, randReal);
            double E_cms = std::pow(A/(1+A), 2);
            // calculate the energy in lab system
            E_lab = (1+A*A+2*A*mu_cms) / ((1+A) * (1+A));
            // convert to lab system
            mu_lab = mu_cms * std::sqrt(E_cms / E_lab) + 1 / (A+1) * std::sqrt(1/E_lab);
        }
    }
    else
    {
        // thermal, free-gas model
        thermalNeutronElasticScatterSampling(A, particle.ergE, E_lab, mu_lab);
    }
    
    // update particle energy
    particle.ergE *= E_lab;
    // update particle's moving direction
    particle.scatter(mu_lab);

    return 0;
}

// int fastNeutronElasticScatterSampling(const double A, double& E_lab, double& mu_lab)
// {
    
// }
int thermalNeutronElasticScatterSampling(const double A, const double E_0, double& E_lab, double& mu_lab)
{
    // References:
    // [1] Monte Carlo Particle Transport Methods: Neutron and Photon Calculations, p72
    // [2] SELECTING THE ENERGY AND SCATTERING ANGLE OF THERMAL NEUTRONS IN FREE GAS MODEL
    const double kT = 0.0253; // room temperature, in eV
    const double lambda = 1/A;
    const double a = std::sqrt(E_0 / (lambda * kT));
    const double g = 1.0 / M_2_SQRTPI * (2*a*a + 1) * std::erf(a);
    const double h = a * std::exp(-a*a);
    double p, q;
    static NormalRandNumGenerator normalRangGen(0, M_SQRT1_2);
    if (GlobalUniformRandNumGenerator::GetInstance().generateDouble() * (g+h) > (g-h))
    {
        // select q with Method Q1
        q = std::sqrt(a*a-std::log(GlobalUniformRandNumGenerator::GetInstance().generateDouble()));
        // select p with Method P1
        double z;
        if (GlobalUniformRandNumGenerator::GetInstance().generateDouble() < a / q)
        {
            // yes
            double R4 = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
            double R5 = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
            z = std::max(R4, R5);
        }
        else
        {
            z = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
        }
        p = 2 * (2*z+q/a-1) / (lambda + 1);
        
    }
    else
    {
        // select q with Method Q2
        bool flag=false;
        if (a < 0.71)
        {
            // yes
            double R1 =  GlobalUniformRandNumGenerator::GetInstance().generateDouble();
            double R2 =  GlobalUniformRandNumGenerator::GetInstance().generateDouble();
            double R3 =  GlobalUniformRandNumGenerator::GetInstance().generateDouble();
            double x = std::max(std::max(R1, R2), R3);
            if (GlobalUniformRandNumGenerator::GetInstance().generateDouble() < std::exp(-std::pow(a*(2*x-1), 2.0)))
            {
                flag = true;
                q = a * (2 * x - 1);
            }
        }
        while(!flag)
        {
            // sample from a normal distribution with mean = 0, stddev = 1/sqrt(2)
            double R = normalRangGen.generateDouble();
            if (std::abs(R) < a && 
                GlobalUniformRandNumGenerator::GetInstance().generateDouble() <= std::pow((R+a)/(2*a), 2))
            {
                // yes
                flag = true;
                q = R;
            }
        }
        // select p with Method P2
        double R4 = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
        double R5 = GlobalUniformRandNumGenerator::GetInstance().generateDouble();
        double z = std::max(R4, R5);
        p = 2*z*(q+a) / (a*(lambda+1));
    }
    E_lab = 2 * p * q / a - lambda * p * p + 1;
    mu_lab = (1+E_lab - p*p) / (2 * std::sqrt(E_lab));
    if (std::abs(mu_lab) > 1)
    {
        mu_lab = 0.99999999999;
    }
    if(std::abs(mu_lab) < -1)
    {
        mu_lab = -0.9999999999;
    }
    
    return 0;
}