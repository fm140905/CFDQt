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
    Sphere detector;
    int NPS=0;
public:
    Tally(const Sphere& s, const int nbins_, const double lower_, const double upper_)
        : detector(s), hist(Histogram(nbins_, lower_, upper_)) {}
    Tally() : Tally(Sphere(QVector3D(0,0,0), 1),100,0,1) {}
    
    void Fill(const Particle& particle, const double prob=1)
    {
        hist.fill(particle.ergE, particle.weight * prob);
    }

    double trackLength(const Particle& particle) const
    {
        Ray ray = Ray(particle.pos, particle.dir);
        return detector.intersection(ray);
    }
    QVector3D getCenter() const {return detector.getCenter();}
    Histogram hist;
    double getMinE() const {return hist.getBinCenter(0);}
    double getMaxE() const {return hist.getBinCenter(hist.getNBins() - 1);}
    // double getArea() const {return 4*M_PI*std::pow(detector.getRadius(), 2.0);}
    double getArea() const {return 1.0;}
    double getVolume() const {return 1.0;}
    double getRadius() const {return detector.getRadius();}
    int getNPS() const {return NPS;}

    void setNPS(const int n) {NPS=n;}
    void setCenter(const QVector3D& newc) {detector.setCenter(newc);}
    void setRadius(const double newr) {detector.setRadius(newr);}
    void reset() {hist.clear();}

};

int primaryContribution(const Particle& particle, const MCSettings& config, Tally& tally);
int scatterContribution(Particle particle, const MCSettings& config, Tally& tally);
int forceDetection(Particle& particle, const MCSettings& config, Tally& tally);
