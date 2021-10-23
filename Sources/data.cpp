#include "data.h"

// getters
int Histogram::getNBins() const
{
    return nbins;
}
double Histogram::getBinContent(const int& binIndex) const
{
    return binCounts[binIndex];
}
double Histogram::getBinCenter(const int& binIndex) const
{
    return binCenters[binIndex];
}
std::vector<double> Histogram::getBinCenters() const
{
    return binCenters;
}
std::vector<double> Histogram::getBinContents() const
{
    return binCounts;
}
int Histogram::getTotalCounts() const
{
    return totalCounts;
}

// setters
void Histogram::setBinContents(const std::vector<double> counts)
{
    if (counts.size() == binCenters.size())
    {
        binCounts = counts;
    }
}

// fill new data
bool Histogram::fill(const double item, const double weight)
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
void Histogram::clear()
{
    totalCounts = 0;
    std::fill(binCounts.begin(), binCounts.end(), 0);
}

// rebin
void Histogram::rebin(const int nbins_, const double lower_, const double upper_)
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