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
    int getNBins() const
    {
        return nbins;
    }
    double getBinContent(const int& binIndex) const
    {
        return binCounts[binIndex];
    }
    double getBinCenter(const int& binIndex) const
    {
        return binCenters[binIndex];
    }
    std::vector<double> getBinCenters() const
    {
        return binCenters;
    }
    std::vector<double> getBinContents() const
    {
        return binCounts;
    }
    int getTotalCounts() const
    {
        return totalCounts;
    }

    // setters
    void setBinContents(const std::vector<double> counts)
    {
        if (counts.size() == binCenters.size())
        {
            binCounts = counts;
        }
    }
    
    // fill new data
    bool fill(const double item, const double weight=1)
    {
        if (item >= upperEdge || item < lowerEdge)
        {
            return false;
        }
        int binIndex = (item - lowerEdge) / binwidth;
//        if(binIndex < 0 || binIndex >= nbins)
//            return false;
        binCounts[binIndex] += weight;
        totalCounts += weight;
        return true;
    }

    // clear
    void clear()
    {
        totalCounts = 0;
        std::fill(binCounts.begin(), binCounts.end(), 0);
    }

    // rebin
    void rebin(const int nbins_, const double lower_, const double upper_)
    {
        totalCounts=0;
        nbins = nbins_;
        lowerEdge = lower_;
        upperEdge = upper_;
        binCounts = std::vector<double>(nbins, 0);
        binCenters = std::vector<double>(nbins, 0);
        binwidth = (upperEdge-lowerEdge) / nbins;
        binCenters[0] = lowerEdge + 0.5 * binwidth;
        for (int i=1;i<nbins;i++)
        {
            binCenters[i] = binCenters[i-1]+binwidth;
        }
    }

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