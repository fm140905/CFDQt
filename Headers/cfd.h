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

class Sphere : public Shape
{
private:
    const QVector3D center;
    const double radius;
public:
    Sphere(const QVector3D& c, const double r) 
        : center(c), radius(r) {}
    // ~Sphere();
    bool contain(const QVector3D& point) const
    {
        return (point-center).length() < radius;
    }
    double intersection(const Ray& ray) const
    {
        double d = center.distanceToLine(ray.getOrigin(), ray.getDirection());
        if(d >= radius)
            return 0;
        return 2 * qSqrt(radius*radius - d * d);
    }
};

/**
 * @brief F2 tally. spherical shape
 * 
 */
class Tally
{
private:
    const Sphere sphere;
    Histogram hist;
public:
    Tally(const Sphere& s, const int nbins_, const double lower_, const double upper_)
        : sphere(s), hist(Histogram(nbins_, lower_, upper_)) {}
    
    void Fill(const Particle& particle, const double prob=1)
    {
        hist.fill(particle.ergE, particle.weight * prob);
    }
};

int CFD(Particle& particle, const MCSettings& config, Tally& tally);