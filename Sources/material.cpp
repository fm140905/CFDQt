#include "material.h"


bool NeutronCrossSection::loadCrossSection(std::string crossSectionFile, std::map<double, double> crossSectionTable)
{
    std::ifstream fileptr;
    fileptr.open(crossSectionFile, std::ios::in);
    if (!fileptr.is_open())
    {
        std::string errMessage = "can't open file: " + crossSectionFile;
        throw std::runtime_error(errMessage);
    }
    std::string line;
    // Read one line at a time into the variable line:
    while(std::getline(fileptr, line))
    {
        // skip lines that starts wth a "#"
        if (line.substr(0, 1) == "#")
        {
            continue;
        }
        
        std::vector<double> lineData;
        std::stringstream lineStream(line);
        double energy, crosssection;
        std::string s;
        while(lineStream >> energy >> crosssection >> s)
        {
            crossSectionTable[energy] = crosssection;
        }
    }
    return true;
}

bool NeutronCrossSection::loadDAFile(std::string DAFile, std::map<double, std::vector<double>> DATable)
{
    std::ifstream fileptr;
    fileptr.open(DAFile, std::ios::in);
    if (!fileptr.is_open())
    {
        std::string errMessage = "can't open file: " + DAFile;
        throw std::runtime_error(errMessage);
    }
    std::string line;
    while(std::getline(fileptr, line))
    {
        std::vector<double> lineData;
        std::stringstream lineStream(line);
        double energy, value;
        std::vector<double> values;
        lineStream >> energy;
        while(lineStream >> value)
        {
            values.push_back(value);
        }
        DATable[energy] = values;
    }
    return true;
}

bool NeutronCrossSection::loadTotalCrossSection(std::string totalCrossSectionFile)
{
    return loadCrossSection(totalCrossSectionFile, totalMicroscopicCrossSection);
}
bool NeutronCrossSection::loadElasticCrossSection(std::string elasticCrossSectionFile)
{
    return loadCrossSection(elasticCrossSectionFile, elasticMicroscopicCrossSection);
}
bool NeutronCrossSection::loadDAPDFFile(std::string DAPDFFile)
{
    return loadDAFile(DAPDFFile, DAPDF);
}
bool NeutronCrossSection::loadDAInverseCDFFile(std::string DAInverseCDFFile)
{
    return loadDAFile(DAInverseCDFFile, DAInverseCDF);
}

const double& NeutronCrossSection::getTotalMicroScopicCrossSectionAt(double energy)
{
    return getClosestEntry(energy, totalMicroscopicCrossSection);
}
const double& NeutronCrossSection::getElasticMicroScopicCrossSectionAt(double energy)
{
    return getClosestEntry(energy, elasticMicroscopicCrossSection);
}
const double& NeutronCrossSection::getDAPDFAt(double energy, double mu)
{
    const std::vector<double>& entry = getClosestEntry(energy, DAPDF);
    int mu_idx = std::floor((mu+1) / 0.02);
    return entry[mu_idx];
}
const double& NeutronCrossSection::getDAInvCDFAt(double energy, double probability)
{
    const std::vector<double>& entry = getClosestEntry(energy, DAInverseCDF);
    double p_idx = std::floor(probability / 0.01);
    
    // linear interpolation
    // double xl = p_idx * 0.01;
    // double xr = xl + 0.01;
    // double yl = entry[static_cast<int>(p_idx)];
    // double yr = entry[static_cast <int>(p_idx) + 1];
    // double val = yl * (xr - probability) + yr * (probability - xl);
    // return val;

    return entry[static_cast<int>(p_idx)];
}