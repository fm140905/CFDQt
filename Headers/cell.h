/**
 * @file cell.h
 * @brief cells and source, MC run setup
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "geometry.h"
#include "data.h"
#include <QRandomGenerator>

class Particle
{
private:
public:
    Particle(const QVector3D& p, const QVector3D& d, const double erg, const double w, const int n, const bool b)
        : pos(p), dir(d), ergE(erg), weight(w), scatterN(n), escaped(b) {}
    Particle(const QVector3D& p, const QVector3D& d, const double erg, const double w)
        : Particle(p, d, erg, w, 0, false) {}

    QVector3D pos;
    QVector3D dir; // unit vector
    bool escaped=false;
    double weight=1;
    double ergE; // energy, Mev
    int scatterN=0;

    void move(const double length) {pos = pos + length * dir;}
};

class Cell
{
private:
    const Material material;
    const double density;
    const Cylinder cylinder;
public:
    Cell(const Material& mat, const double d, const Cylinder& cyl)
        : material(mat),
          density(d),
          cylinder(cyl)
        {}
    
    bool contains(const Particle& p) {return cylinder.contain(p.pos);}
};

class Source
{
private:
    const Cylinder cylinder;
    const Histogram energyCDF;
public:
    Source(const Cylinder& cyl, const Histogram& ergCDF)
        : cylinder(cyl), energyCDF(ergCDF) {}
    // ~Source();

    Particle createParticle() const;
};


class MCSettings
{
private:
    const Cylinder& cylinder;
    const Source& source;
    const int maxN;
    const int maxScatterN;
    const double minW;
    const double minE;
public:
    MCSettings(const Cylinder& cyl, const Source& src, const int maxn, 
               const int maxscattern, const double minw, const double mine)
        : cylinder(cyl), source(src), maxN(maxn), maxScatterN(maxscattern),
          minW(minw), minE(mine)
    {}
    // ~MCSettings();
};