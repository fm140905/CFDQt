/**
 * @file cell.h
 * @brief cells and source
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
    const Material& material;
    const double density;
    const Cylinder& cylinder;
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
    const Cylinder& cylinder;
    const Histogram& energyCDF;
public:
    Source(const Cylinder& cyl, const Histogram& ergCDF)
        : cylinder(cyl), energyCDF(ergCDF) {}
    // ~Source();

    Particle createParticle() const
    {
        // uniform, [0. 1)
        double initZ = QRandomGenerator::global()->generateDouble() * cylinder.getHeight();
        double initX, initY;
        do
        {
            initX = cylinder.getRadius() * QRandomGenerator::global()->generateDouble();
            initY = cylinder.getRadius() * QRandomGenerator::global()->generateDouble();
        } while (initX * initX + initY * initY >= qPow(cylinder.getRadius(), 2));
        QVector3D initPos = QVector3D(initX, initY, initZ) + cylinder.getBaseCenter();

        double phi= 2 * M_PI * QRandomGenerator::global()->generateDouble();
        double theta = qAcos(1 - 2 * QRandomGenerator::global()->generateDouble());
        QVector3D initDir = QVector3D(qCos(phi) * qSin(theta), qSin(phi) * qSin(theta), qCos(theta));

        double initE = 0;
        for (std::size_t i = 0; i < energyCDF.getNBins(); i++)
        {
            if( QRandomGenerator::global()->generateDouble() < energyCDF.getBinContent(i))
            {
                initE = energyCDF.getBinCenter(i);
                break;
            }
        }
        return Particle(initPos, initDir, initE, 1.0);
    }
};

