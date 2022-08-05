/**
 * @file cell.h
 * @brief cells and source, MC run setup
 * @version 0.1
 * @date 2021-07-26
 * 
 * @author Ming Fang
 * 
 */

#pragma once

#include "geometry.h"
#include "data.h"
#include "material.h"
#include "rng.h"

class Particle
{
private:
public:
    /**
     * @brief Construct a new Particle
     * 
     * @param p initial position
     * @param d initial moving direction, normalized
     * @param erg initial energy. MeV for gamma, eV for neutron
     * @param w initial weight
     * @param n initial number of scatterings
     * @param b whether particle is outside of ROI
     */
    Particle(const QVector3D& p, const QVector3D& d, const double erg, const double w, const int n, const bool b)
        : pos(p), dir(d.normalized()), ergE(erg), weight(w), scatterN(n), escaped(b) 
    {
        initpos = pos;
    }
    /**
     * @brief Construct a new Particle object
     * 
     * @param p initial position
     * @param d initial moving direction, normalized
     * @param erg initial energy. eV for gamma, MeV for neutron
     * @param w initial weight
     */
    Particle(const QVector3D& p, const QVector3D& d, const double erg, const double w)
        : Particle(p, d, erg, w, 0, false) {}
    // initial position
    QVector3D initpos; // for debugging
    // current position
    QVector3D pos;
    // current moving direction, unit vector
    QVector3D dir;
    // whether particle is outside of ROI
    bool escaped=false;
    // current particle weight
    double weight=1;
    // current particle energy, Mev for gamma, eV for neutron
    double ergE;
    // total number of scattering since creation
    int scatterN=0;

    /**
     * @brief Update particle position when it moves along current direction with given distance
     * 
     * @param length Distance the particle should travel
     */
    void move(const double length) {pos = pos + length * dir;}

    /**
     * @brief Update moving direction when the particle scatters with given scattering angle
     * 
     * @param mu Cosine of the scattering angle
     */
    void scatter(const double mu);
};

class Cell
{
private:
    // density of the material in cell, g/cc
    const double density;
    // shape of the cell
    const Cylinder cylinder;
public:
    /**
     * @brief Construct a new Cell object
     * 
     * @param mat material in the cell
     * @param d density of the material, g/cc
     * @param cyl shape of the cell
     */
    Cell(const Material& mat, const double d, const Cylinder& cyl)
        : material(mat),
          density(d),
          cylinder(cyl)
        {}
    
    const Material material;
    
    /**
     * @brief Check if a particle is in this cell
     * 
     * @param p paticle to be checked
     * @return true if the particle is in this cell.
     * @return false else
     */
    bool contains(const Particle& p) const {return cylinder.contain(p.pos);}
};

class Source
{
private:
    const Cylinder cylinder;
    // inverse function of the cumulative probabilistic energy distribution
    const std::vector<double> invCDF;
    // width of the CDF bin
    double CDFBinWidth;
public:
    /**
     * @brief Construct a new Source object
     * 
     * @param cyl Source shape. In this case it is a cylinder
     * @param ergCDF Inverse function of the cumulative probabilistic energy distribution.
     *               A vector of energies. If only one element, the source is monoergetic.
     *               If N elements, there are N-1 equal-probable bins and 
     *               each elment E_i satifies P(E<E_i) = i/(N-1), i=0, ..., N-1
     */
    Source(const Cylinder& cyl, const std::vector<double>& ergCDF)
        : cylinder(cyl), invCDF(ergCDF) 
        {
            if (invCDF.size() > 1)
            {
                // N euqal-probable bins
                CDFBinWidth = 1.0 / (invCDF.size() - 1);
            }
        }
    // ~Source();

    /**
     * @brief Create a Particle object. Position and direction uniformly sampled.
     *        Energy sampled based on the source energy CDF. Weight set to 1.
     * 
     * @return Particle Newly-created particle
     */
    Particle createParticle() const;
};


class MCSettings
{
    // TODO: Implement MaxAtten for neutrons as well.
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

        /**
         * @brief Get the maximum total attenuation coefficient for all materials present at given energy.
         * 
         * @param erg Energy
         * @return double Maximum total attenuation coefficient
         */
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
    /**
     * @brief Construct a new MCSettings object
     * 
     * @param cyl ROI, including all cells
     * @param cels List of cells
     * @param src Particle source
     * @param maxn Number of particles to run (NPS).
     * @param maxscattern Max number of scatterings allowed. 
     *                    Stop tracking current particle if scattering number is larger than this and go to next particle.
     * @param minw Minimum of particle weight allowed. 
     *             Stop tracking current particle if weight is smaller than this and go to next particle.
     * @param mine Minimum particle energy allowed. 
     *             Stop tracking current particle if energy is smaller than this and go to next particle.
     */
    MCSettings(const Cylinder& cyl, std::vector<Cell> cels, const Source& src, const int maxn, 
               const int maxscattern, const double minw, const double mine)
        : ROI(cyl), cells(cels), source(src), maxN(maxn), maxScatterN(maxscattern),
          minW(minw), minE(mine)
    {
        std::vector<double> maxatten;
        std::vector<double> ergs;
        // build the list of max total attenuation by iterating over all energies and all materials.
        for (std::size_t i = 0; i < cells[0].material.getPhotonCrossSection().getNbins(); i++)
        {
            double muMax(0);
            double erg = cells[0].material.getPhotonCrossSection().getEBinCenter(i);
            for (std::size_t j = 0; j < cells.size(); j++)
            {
                if (muMax < cells[j].material.getPhotonTotalAtten(erg))
                {
                    muMax = cells[j].material.getPhotonTotalAtten(erg);
                }
            }
            ergs.push_back(erg);
            maxatten.push_back(muMax);
        }
        maxAtten = MaxAtten(ergs, maxatten);
    }
    // region of interest
    const Cylinder ROI;
    // list of cells
    const std::vector<Cell> cells;
    const Source source;
    const int maxN;
    const int maxScatterN;
    const double minW;
    const double minE;

    /**
     * @brief Get the maximum total attenuation coefficient for all materials at given energy.
     * 
     * @param erg Energy
     * @return double Max total attenuation coefficient.
     */
    double getMuMax(const double erg) const {return maxAtten.getMaxAtten(erg);}
};