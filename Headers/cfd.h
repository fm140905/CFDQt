/**
 * @file cfd.h
 * @brief forced detection
 * @version 0.1
 * @date 2021-07-26
 * 
 * @author Ming Fang
 * 
 */
#pragma once

#include "geometry.h"
#include "data.h"
#include "tracking.h"

/**
 * @brief F2 tally. Spherical shape
 * 
 */
class Tally
{
private:
    const Sphere detector;
    Histogram hist;
    // true if using letharg bins
    const bool letharg;
public:
    /**
     * @brief Construct a new Tally object
     * 
     * @param s Detector shape
     * @param nbins_ Number of energy bins
     * @param lower_ Lower energy limit
     * @param upper_ Upper energy limit
     */
    Tally(const Sphere& s, const int nbins_, const double lower_, const double upper_)
        : Tally(s, nbins_, lower_, upper_, false) {}
    
    /**
     * @brief Construct a new Tally object
     * 
     * @param s Detector shape
     * @param nbins_ Number of energy bins
     * @param lower_ Lower energy limit
     * @param upper_ Upper energy limit
     * @param letharg_ Whether using lethargy bins
     */
    Tally(const Sphere& s, const int nbins_, const double lower_, const double upper_, bool letharg_)
        : detector(s), letharg(letharg_) 
        {
            if(letharg)
                hist = Histogram(nbins_, std::log10(lower_), std::log10(upper_));
            else
                hist = Histogram(nbins_, lower_, upper_);
        }
    
    /**
     * @brief Update tally counts when the particle will be detected with a given probability.
     * 
     * @param particle Particle to be detected
     * @param prob Probability of detecting the particle
     */
    void Fill(const Particle& particle, const double prob=1)
    {
        if (isnan(prob))
            return;
        if (letharg)
            hist.fill(std::log10(particle.ergE), particle.weight * prob);
        else
            hist.fill(particle.ergE, particle.weight * prob);
    }

    /**
     * @brief Get the length of the particle track in detector volume
     * 
     * @param particle Particle to be detected
     * @return double Track legnth
     */
    double trackLength(const Particle& particle) const
    {
        Ray ray = Ray(particle.pos, particle.dir);
        return detector.intersection(ray);
    }

    QVector3D getCenter() const 
    {
        return detector.getCenter();
    }
    int getNBins() const {return hist.getNBins();}
    double getBinCenter(int binIdx) const 
    {
        if (letharg)
            return std::pow(10, hist.getBinCenter(binIdx));
        else
            return hist.getBinCenter(binIdx);
    }
    double getBinContent(int binIdx) const {return hist.getBinContent(binIdx);}
    /**
     * @brief Get the lower energy limit of tally
     * 
     * @return double 
     */
    double getMinE() const 
    {
        if (letharg)
            return std::pow(10, hist.getBinCenter(0));
        else
            return hist.getBinCenter(0);
    }
    /**
     * @brief Get the upper energy limit of tally
     * 
     * @return double 
     */
    double getMaxE() const 
    {
        if (letharg)
            return std::pow(10, hist.getBinCenter(hist.getNBins() - 1));
        else
            return hist.getBinCenter(hist.getNBins() - 1);
    }
    /**
     * @brief Get the width of the i-th energy bin
     * 
     * @param binIdx Index of energy bin
     * @return double Energy bin width
     */
    double getBinWidth(int binIdx) const
    {
        if (letharg)
            return std::pow(10, hist.getBinCenter(binIdx) + hist.getBinWidth() / 2.0) - std::pow(10, hist.getBinCenter(binIdx) - hist.getBinWidth() / 2.0);
        else
            return hist.getBinWidth();
    }
    double getArea() const {return 1.0;}
    /**
     * @brief Get the detector volume
     * 
     * @return double 
     */
    double getVolume() const {return 1.0;}
    /**
     * @brief Get the detector radius
     * 
     * @return double 
     */
    double getRadius() const {return detector.getRadius();}
};

/**
 * @brief Update tally counts contributed by a newly-created photon (primary contribution)
 * 
 * @param particle A newly-created photon
 * @param config MC run settings
 * @param tally Energy tally
 * @return int 
 */
int primaryContribution(const Particle& particle, const MCSettings& config, Tally& tally);
/**
 * @brief Update tally counts if a photon were scattered towards the detector
 * 
 * @param particle Photon to be scattered
 * @param config MC run settings
 * @param tally Tally to be updated
 * @return int 
 */
int scatterContribution(Particle particle, const MCSettings& config, Tally& tally);
/**
 * @brief Update tally counts when the photon is forced to travel towards the detector
 * 
 * @param particle Current photon
 * @param config MC run settings
 * @param tally Tally to be updated
 * @return int 
 */
int forceDetection(Particle& particle, const MCSettings& config, Tally& tally);

/**
 * @brief Update tally counts when the neutron is forced to travel towards the detector
 * 
 * @param particle Current neutron
 * @param config MC run settings
 * @param tally Tally to be updated
 * @return int 
 */
int forceDetectionNeutron(Particle& particle, const MCSettings& config, Tally& tally);
/**
 * @brief Update tally counts contributed by a newly-created neutron (primary contribution)
 * 
 * @param particle A newly-created neutron
 * @param config MC run settings
 * @param tally Tally to be updated
 * @return int 
 */
int primaryContributionNeutron(const Particle& particle, const MCSettings& config, Tally& tally);
/**
 * @brief Update tally counts if a fast neutron were scattered towards the detector
 * 
 * @param particle Fast neutron to be scattered
 * @param config MC run settings
 * @param tally Tally to be updated
 * @return int 
 */
int scatterContributionNeutron(Particle particle, const MCSettings& config, Tally& tally);
/**
 * @brief Update tally counts if a thermal neutron were scattered towards the detector
 * 
 * @param particle Thermal neutron to be scattered
 * @param config MC run settings
 * @param tally Tally to be updated
 * @return int 
 */
int scatterContributionThermalNeutron(Particle particle, const MCSettings& config, Tally& tally);