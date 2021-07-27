/**
 * @file data.h
 * @author ming (mingf2@illinois.edu)
 * @brief load cross-section libraries
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Histogram
{
public:
    Histogram() {}
    Histogram(const int nbins_, const double lower_, const double upper_)
        : nbins(nbins_),
          lowerEdge(lower_),
          upperEdge(upper_)
    {
        rebin(nbins, lowerEdge, upperEdge);
    }

    // getters
    int getNBins() const;
    double getBinContent(const int& binIndex) const;
    double getBinCenter(const int& binIndex) const;
    std::vector<double> getBinCenters() const;
    std::vector<double> getBinContents() const;
    int getTotalCounts() const;

    // setters
    void setBinContents(const std::vector<double> counts);
    
    // fill new data
    bool fill(const double item, const double weight=1);

    // clear
    void clear();

    // rebin
    void rebin(const int nbins_, const double lower_, const double upper_);

private:
    int nbins;
    double lowerEdge;
    double upperEdge;
    double binwidth;
    std::vector<double> binCenters;
    std::vector<double> binCounts;
    int totalCounts;
};

class Material
{
private:
    int matID;
    // center of energy bins, MeV
    std::vector<float> EBinCenters;
    // integrated Compton cross section over 4pi slod angle
    std::vector<float> IntegralComptonCrossSection;
    // Compton over total
    std::vector<float> ComptonOverTotal;
    // total attenuation coefficient, cm^{-1}
    std::vector<float> totalAtten;
    // min energy
    float Emin;
    float Emax;
    float deltaE;
    int NEbin;
public:
    // constructor
    Material(const std::string fpath, const int id);
    // ~Material();

    float getAtten(const double erg) const;
    float getTotalComptonIntegral(const double erg) const;
    float getComptonOverTotal(const double erg) const;
};

#endif // DATA_H