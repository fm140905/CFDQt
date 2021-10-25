#include "cfd.h"

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

    // F1 tally
    double score = 1;
    // // F2 tally
    // double score = 1 / (tally.getArea() * std::sqrt(1-std::pow(d / tally.getRadius(), 2.0)));
    // // F4 tally
    // double score = 2 * std::sqrt(std::pow(tally.getRadius(), 2.0) - std::pow(d, 2.0)) / tally.getVolume();
    
    
    // attenuation along the ray
    Ray ray = Ray(particle.pos, prtl2det);
    double atten = config.ROI.intersection(ray) * config.cells[0].material.getAtten(particle.ergE);

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
    double atten = config.ROI.intersection(ray) * config.cells[0].material.getAtten(newErg);

    // K-N equation 
    double sigma = std::pow(beta, 2) * (beta + 1/beta + std::pow(cosAng, 2) - 1) / config.cells[0].material.getTotalComptonIntegral(newErg);

    double ratio = tally.getRadius() / length;


    // contribution to tally F1
    double score = 2 * sigma * (1-std::sqrt(1-ratio*ratio));
    // // contribution to tally F2, integrated over the spherical surface
    // double score = 1 / tally.getArea() * sigma * ratio * std::log((1+ratio) / (1-ratio));
    // // contribution to tally F4
    // double avgTrackLength = 4*tally.getRadius()/3.0;
    // double score = 2 * sigma * (1-std::sqrt(1-ratio*ratio)) * avgTrackLength / tally.getVolume();

    particle.ergE *= beta;
    tally.Fill(particle, std::exp(-atten)* score);

    return 0;
}