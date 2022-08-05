/**
 * @file data.h
 * @author ming (mingf2@illinois.edu)
 * @brief load cross-section libraries
 * @version 0.1
 * @date 2021-07-26
 * 
 * @author Ming Fang
 * 
 */
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
// #include <filesystem>

class Histogram
{
public:
    Histogram() {}
    /**
     * @brief Construct a new Histogram object
     * 
     * @param nbins_ Number of equal-size bins
     * @param lower_ Left edge of first bin 
     * @param upper_ Right edge of last bin 
     */
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
    double getBinWidth() const {return binwidth;}

    // setters
    void setBinContents(const std::vector<double> counts);
    void scaling(const double f);
    
    // fill new data
    bool fill(const double item, const double weight=1);

    // clear bin data
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

// std::string getRootDir();