/**
 * @file cfd.h
 * @brief forced detection
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include "geometry.h"
#include "data.h"
#include "tracking.h"

/**
 * @brief F2 tally. spherical shape
 * 
 */
class Tally
{
private:
    const Sphere detector;
    Histogram hist;
    const bool letharg;
public:
    Tally(const Sphere& s, const int nbins_, const double lower_, const double upper_)
        : Tally(s, nbins_, lower_, upper_, false) {}
    
    Tally(const Sphere& s, const int nbins_, const double lower_, const double upper_, bool letharg_)
        : detector(s), letharg(letharg_) 
        {
            if(letharg)
                hist = Histogram(nbins_, std::log10(lower_), std::log10(upper_));
            else
                hist = Histogram(nbins_, lower_, upper_);
        }
    
    void Fill(const Particle& particle, const double prob=1)
    {
        if (isnan(prob))
            return;
        if (letharg)
            hist.fill(std::log10(particle.ergE), particle.weight * prob);
        else
            hist.fill(particle.ergE, particle.weight * prob);
    }

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
    double getMinE() const 
    {
        if (letharg)
            return std::pow(10, hist.getBinCenter(0));
        else
            return hist.getBinCenter(0);
    }
    double getMaxE() const 
    {
        if (letharg)
            return std::pow(10, hist.getBinCenter(hist.getNBins() - 1));
        else
            return hist.getBinCenter(hist.getNBins() - 1);
    }
    double getBinWidth(int binIdx) const
    {
        if (letharg)
            return std::pow(10, hist.getBinCenter(binIdx) + hist.getBinWidth() / 2.0) - std::pow(10, hist.getBinCenter(binIdx) - hist.getBinWidth() / 2.0);
        else
            return hist.getBinWidth();
    }
    // double getArea() const {return 4*M_PI*std::pow(detector.getRadius(), 2.0);}
    double getArea() const {return 1.0;}
    double getVolume() const {return 1.0;}
    double getRadius() const {return detector.getRadius();}
};

int primaryContribution(const Particle& particle, const MCSettings& config, Tally& tally);
int scatterContribution(Particle particle, const MCSettings& config, Tally& tally);
int forceDetection(Particle& particle, const MCSettings& config, Tally& tally);

int forceDetectionNeutron(Particle& particle, const MCSettings& config, Tally& tally);
int primaryContributionNeutron(const Particle& particle, const MCSettings& config, Tally& tally);
int scatterContributionNeutron(Particle particle, const MCSettings& config, Tally& tally);
int scatterContributionThermalNeutron(Particle particle, const MCSettings& config, Tally& tally);