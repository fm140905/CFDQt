#include "cfd.h"
#include <iostream>

int forceDetection(Particle& particle, const MCSettings& config, Tally& tally)
{
    if (particle.scatterN == 0)
    {
        primaryContribution(particle, config, tally);
    }
    else
    {
        scatterContribution(particle, config, tally);
    }
    return 0;
}

int primaryContribution(const Particle& particle, const MCSettings& config, Tally& tally)
{
    QVector3D prtl2det = tally.getCenter() - particle.pos;
    double proj = QVector3D::dotProduct(prtl2det, particle.dir);
    if(proj <= 0)
        return 0;
    
    double d = tally.getCenter().distanceToLine(particle.pos, particle.dir);
    if (d >= tally.getRadius())
        return 0;

    // // F1 tally
    // double score = 1;
    // // F2 tally
    // double score = 1 / (tally.getArea() * std::sqrt(1-std::pow(d / tally.getRadius(), 2.0)));
    // F4 tally
    double score = 2 * std::sqrt(std::pow(tally.getRadius(), 2.0) - std::pow(d, 2.0)) / tally.getVolume();
    
    
    // attenuation along the ray
    Ray ray = Ray(particle.pos, particle.dir);
    double atten = config.ROI.intersection(ray) * config.cells[0].material.getPhotonTotalAtten(particle.ergE);

    tally.Fill(particle, std::exp(-atten)*score);
    
    return 0;
}

int scatterContribution(Particle particle, const MCSettings& config, Tally& tally)
{
    // determine the scattering angle if the particle
    // were scattered towards the detector
    QVector3D prtl2det = tally.getCenter() - particle.pos;
    double length = prtl2det.length();
    prtl2det.normalize();
    double cosAng = QVector3D::dotProduct(particle.dir, prtl2det);

    // new energy / pre energy
    double beta = 1 / (1+particle.ergE / 0.511 * (1-cosAng));
    double newErg = beta * particle.ergE;
    if (newErg < tally.getMinE() ||
        newErg > tally.getMaxE())
    {
        return 0;
    }
    
    // attenuation along the ray
    Ray ray = Ray(particle.pos, prtl2det);
    double atten = config.ROI.intersection(ray) * config.cells[0].material.getPhotonTotalAtten(newErg);

    // K-N equation 
    double sigma = std::pow(beta, 2) * (beta + 1/beta + std::pow(cosAng, 2) - 1) / config.cells[0].material.getPhotonCrossSection().getTotalComptonIntegral(newErg);

    double ratio = tally.getRadius() / length;


    // // contribution to tally F1
    // double score = 2 * sigma * (1-std::sqrt(1-ratio*ratio));
    // // contribution to tally F2, integrated over the spherical surface
    // double score = 1 / tally.getArea() * sigma * ratio * std::log((1+ratio) / (1-ratio));
    // contribution to tally F4
    // double avgTrackLength = 4*tally.getRadius()/3.0;
    // double score = 2 * sigma * (1-std::sqrt(1-ratio*ratio)) * avgTrackLength / tally.getVolume();
    double lbda= ratio - 0.5 * (1-ratio*ratio) * std::log((1+ratio) / (1-ratio));
    double score = 2 * sigma * length * lbda / tally.getVolume();

    particle.ergE *= beta;
    tally.Fill(particle, std::exp(-atten)* score);

    return 0;
}

int forceDetectionNeutron(Particle& particle, const MCSettings& config, Tally& tally)
{
    if (particle.scatterN == 0)
    {
        primaryContributionNeutron(particle, config, tally);
    }
    else
    {
        scatterContributionNeutron(particle, config, tally);
    }
    return 0;
}


int primaryContributionNeutron(const Particle& particle, const MCSettings& config, Tally& tally)
{
    QVector3D prtl2det = tally.getCenter() - particle.pos;
    double proj = QVector3D::dotProduct(prtl2det, particle.dir);
    if(proj <= 0)
        return 0;
    
    double d = tally.getCenter().distanceToLine(particle.pos, particle.dir);
    if (d >= tally.getRadius())
        return 0;

    // F1 tally
    double score = 1;
    // // F2 tally
    // double score = 1 / (tally.getArea() * std::sqrt(1-std::pow(d / tally.getRadius(), 2.0)));
    // // F4 tally
    // double score = 2 * std::sqrt(std::pow(tally.getRadius(), 2.0) - std::pow(d, 2.0)) / tally.getVolume();
    
    
    // attenuation along the ray
    Ray ray = Ray(particle.pos, particle.dir);
    double atten = config.ROI.intersection(ray) * config.cells[0].material.getNeutronTotalAtten(particle.ergE);

    tally.Fill(particle, std::exp(-atten)*score);
    
    return 0;
}

int scatterContributionNeutron(Particle particle, const MCSettings& config, Tally& tally)
{
    // determine the scattering angle if the particle
    // were scattered towards the detector
    QVector3D prtl2det = tally.getCenter() - particle.pos;
    const double length = prtl2det.length();
    prtl2det.normalize();
    const double cosAng = QVector3D::dotProduct(particle.dir, prtl2det); // mu_lab
    
    // attenuation along the ray
    Ray ray = Ray(particle.pos, prtl2det);
    const double attenLength = config.ROI.intersection(ray);

    const double ratio = tally.getRadius() / length;

    // average F1 integrated over the solid angle subtended by detector = 2pi * averageScore,
    // the factor 2pi cancels out with the factor 1/2pi in pdf of angular distribution
    const double averageScore = 1 - std::sqrt(1 - ratio * ratio);

    // average F2 integrated over the solid angle subtended by detector = 2pi * averageScore,
    // the factor 2pi cancels out with the factor 1/2pi in pdf of angular distribution
    // const double averageScore = 0.5 * ratio * std::log((1+ratio)/(1-ratio));

    // // average F4 integrated over the solid angle subtended by detector = 2pi * averageScore,
    // // the factor 2pi cancels out with the factor 1/2pi in pdf of angular distribution
    // const double averageScore = length / tally.getVolume() * 
    //                     (ratio - 0.5 * (1-ratio*ratio) * std::log((1+ratio)/(1-ratio)));

    // iterate all nuclides that the neutron can interact with
    const int nuclidesNum = config.cells[0].material.getNuclidesNumber();
    std::vector<double> scatterNuclideProbs(nuclidesNum, 0);
    std::vector<double> unattenProbs(nuclidesNum, 0);
    std::vector<double> scores(nuclidesNum, 0);
    std::vector<double> E_labs(nuclidesNum, 0);
    int nuclideIdx(0);
    double E_cms(0);
    double E_lab(0);
    double mu_cms(0);
    double pdf(0);
    double dmu_cm_over_du_lab(0);
    for (auto &&comp : config.cells[0].material.getNuclides())
    {
        const Nuclide& nuclide = comp.second;
        const double A = nuclide.getAtomicWeight();
        // special case, H-1
        if (std::abs(A - 1) < 0.1)
        {
            if(cosAng < 0)
            {
                nuclideIdx ++;
                continue; // back-scatter is not possible
            }
            // E_cms = 0.25;
            E_lab = cosAng * cosAng;
            if (E_lab * particle.ergE < tally.getMinE() ||
                E_lab * particle.ergE > tally.getMaxE())
            {
                continue;
            }
            pdf = 0.5;
            dmu_cm_over_du_lab = 4 * cosAng;
        }
        else
        {
            mu_cms = (cosAng * std::sqrt(A*A-1+cosAng*cosAng) - 1 + cosAng*cosAng) / A; 
            if(std::abs(mu_cms) > 1.0)
            {
                // mu_cms = 1.0;
                // throw std::runtime_error(std::string("Cannot find mu_cms for mu_lab = ") + std::to_string(cosAng));
                std::cout << std::string("Cannot find mu_cms for mu_lab = ") + std::to_string(cosAng) << '\n';
                continue;
            }
            // neutron energy in cms
            E_cms = std::pow(A/(A+1), 2);
            E_lab = (1+A*A+2*A*mu_cms) / std::pow(A+1, 2);
            if (E_lab * particle.ergE < tally.getMinE() ||
                E_lab * particle.ergE > tally.getMaxE())
            {
                continue;
            }
            pdf = nuclide.getNeutronCrossSection().getDAPDFAt(particle.ergE, mu_cms);
            dmu_cm_over_du_lab = std::sqrt(E_lab / E_cms) / (1-cosAng / (A+1) * std::sqrt(1/E_lab));

        }
        // probability that neutron scatters by nuclide i 
        scatterNuclideProbs[nuclideIdx] = comp.first * 
                nuclide.getNeutronCrossSection().getElasticMicroScopicCrossSectionAt(particle.ergE);
        // energy of scattered neutron in lab system
        E_lab *= particle.ergE;
        E_labs[nuclideIdx] = E_lab;
        // probablity that neutron can reach detector without being attenuated
        unattenProbs[nuclideIdx] = std::exp(-attenLength * config.cells[0].material.getNeutronTotalAtten(E_lab));
        // F4 tally, PDF(u_cm) * du_cm / du_lab * average constribution integrated over detector sphere
        scores[nuclideIdx] = pdf * dmu_cm_over_du_lab * averageScore;
        nuclideIdx ++;
    }

    // for normalizing probablities of scattering with each nuclide
    // double totalElasticCrossSection = std::accumulate(scatterNuclideProbs.begin(), scatterNuclideProbs.end(), 0);
    double totalTotalCrossSection = config.cells[0].material.getNeutronTotalMicroscopicCrossSection(particle.ergE);
    for (int i = 0; i < nuclidesNum; i++)
    {
        particle.ergE = E_labs[i];
        tally.Fill(particle, scatterNuclideProbs[i] / totalTotalCrossSection * unattenProbs[i] * scores[i]);
    }
    return 0;
}