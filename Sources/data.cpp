#include "data.h"

Material::Material(const std::string fpath, const double d, const int id)
    : density(d), matID(id)
{
    // read attenuation coeffcients saved in txt
    std::ifstream fileptr;
    fileptr.open(fpath, std::ios::in);
    if (!fileptr.is_open())
    {
        // std::cout << "can't open file " << pinLocPath << std::endl;
        // exit(1);
        std::string errMessage = "can't open file: " + fpath;
        throw std::runtime_error(errMessage);
    }
    std::string line;
    // skip headers
    std::getline(fileptr, line);
    std::getline(fileptr, line);
    // Read one line at a time into the variable line:
    while(std::getline(fileptr, line))
    {
        std::vector<double> lineData;
        std::stringstream lineStream(line);

        double value;
        // Read an double at a time from the line
        while(lineStream >> value)
        {
            lineData.push_back(value);
        }
        EBinCenters.push_back(lineData[0]);
        IntegralComptonCrossSection.push_back(lineData[1]);
        ComptonOverTotal.push_back(lineData[2]);
        totalAtten.push_back(lineData[3] * density);
    }
    NEbin = EBinCenters.size();
    Emin = EBinCenters[0];
    Emax = EBinCenters[NEbin - 1];
    deltaE = (Emax - Emin) / (NEbin -1);
}

double Material::getAtten(const double erg) const
{
    int binNum = (erg - Emin) / deltaE + 0.5;
    if (binNum <= 0)
        return totalAtten[0];
    else if (binNum >= NEbin - 1)
        return totalAtten[NEbin - 1];
    return totalAtten[binNum];
    
}
double Material::getTotalComptonIntegral(const double erg) const
{
    int binNum = (erg - Emin) / deltaE + 0.5;
    if (binNum <= 0)
        return IntegralComptonCrossSection[0];
    else if (binNum >= NEbin - 1)
        return IntegralComptonCrossSection[NEbin - 1];
    return IntegralComptonCrossSection[binNum];
}
double Material::getComptonOverTotal(const double erg) const
{
    int binNum = (erg - Emin) / deltaE + 0.5;
    if (binNum <= 0)
        return ComptonOverTotal[0];
    else if (binNum >= NEbin - 1)
        return ComptonOverTotal[NEbin - 1];
    return ComptonOverTotal[binNum];
}

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