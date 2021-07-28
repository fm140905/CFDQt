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
        : pos(p), dir(d.normalized()), ergE(erg), weight(w), scatterN(n), escaped(b) 
    {
        initpos = pos;
    }
    Particle(const QVector3D& p, const QVector3D& d, const double erg, const double w)
        : Particle(p, d, erg, w, 0, false) {}

    QVector3D initpos; // for debugging
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
    const double density;
    const Cylinder cylinder;
public:
    Cell(const Material& mat, const double d, const Cylinder& cyl)
        : material(mat),
          density(d),
          cylinder(cyl)
        {}
    
    const Material material;
    bool contains(const Particle& p) const {return cylinder.contain(p.pos);}
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
    class MaxAtten
    {
    private:
        // center of energy bins, MeV
        std::vector<double> EBinCenters;
        // total attenuation coefficient, cm^{-1}
        std::vector<double> totalAtten;
        double Emin;
        double Emax;
        double deltaE;
        int NEbin;
    public:
        MaxAtten(std::vector<double> ergs, std::vector<double> attens)
            : EBinCenters(ergs), totalAtten(attens)
        {
            Emin = EBinCenters[0];
            NEbin = EBinCenters.size();
            deltaE = EBinCenters[1] - EBinCenters[0];
            Emax = EBinCenters[NEbin - 1];
        }
        MaxAtten(): MaxAtten(std::vector<double>{0}, std::vector<double>{0}) {} 
        // ~MaxAtten();
        double getMaxAtten(const double erg) const
        {
            int binNum = (erg - Emin) / deltaE + 0.5;
            if (binNum <= 0)
                return totalAtten[0];
            else if (binNum >= NEbin - 1)
                return totalAtten[NEbin - 1];
            return totalAtten[binNum];
        }
    };
    
    MaxAtten maxAtten;
public:
    MCSettings(const Cylinder& cyl, std::vector<Cell> cels, const Source& src, const int maxn, 
               const int maxscattern, const double minw, const double mine)
        : ROI(cyl), cells(cels), source(src), maxN(maxn), maxScatterN(maxscattern),
          minW(minw), minE(mine)
    {
        std::vector<double> maxatten;
        std::vector<double> ergs;
        for (std::size_t i = 0; i < cells[0].material.getNbins(); i++)
        {
            double muMax(0);
            double erg = cells[0].material.getEBinCenter(i);
            for (std::size_t j = 0; j < cells.size(); j++)
            {
                if (muMax < cells[j].material.getAtten(erg))
                {
                    muMax = cells[j].material.getAtten(erg);
                }
            }
            ergs.push_back(erg);
            maxatten.push_back(muMax);
        }
        maxAtten = MaxAtten(ergs, maxatten);
    }
    // ~MCSettings();
    const Cylinder& ROI; // region of interest
    const std::vector<Cell> cells;
    const Source& source;
    const int maxN;
    const int maxScatterN;
    const double minW;
    const double minE;

    double getMuMax(const double erg) const {return maxAtten.getMaxAtten(erg);}
};