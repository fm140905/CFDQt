#include "material.h"
#include <iostream>

bool NeutronCrossSection::loadCrossSection(std::string crossSectionFile, std::map<double, double>& crossSectionTable)
{
    std::ifstream fileptr;
    fileptr.open(crossSectionFile, std::ios::in);
    if (!fileptr.is_open())
    {
        std::string errMessage = "can't open file: " + crossSectionFile;
        throw std::runtime_error(errMessage);
    }
    std::string line;
    // int recordNum(0);
    // Read one line at a time into the variable line:
    while(std::getline(fileptr, line))
    {
        // skip lines that starts wth a "#"
        if (line.substr(0, 1) == "#")
        {
            continue;
        }

        std::stringstream lineStream(line);
        double energy, crosssection;
        // std::string s;
        while(lineStream >> energy >> crosssection)
        {
            // crossSectionTable.insert({energy, crosssection});
            auto pos = crossSectionTable.find(energy);
            if (pos == crossSectionTable.end())
            {
                crossSectionTable.insert({energy, crosssection});
            }
            else
            {
                std::cout << "Key in cross-section table exists: " << pos->first << '\n';
            }
        }
    }
    return true;
}

bool NeutronCrossSection::loadDAFile(std::string DAFile, std::map<double, std::vector<double>>& DATable)
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
        lineStream >> energy;
        while(lineStream >> value)
        {
            lineData.push_back(value);
        }
        auto pos = DATable.find(energy);
        if (pos == DATable.end())
        {
            DATable.insert({energy, lineData});
        }
        else
        {
            std::cout << "Key in DA table exists: " << pos->first << '\n';
        }
        
        
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
    if (DAPDFFile.size() == 0)
    {
        // uniform distribution on [-1,1]
        DAPDF.insert({1.5e7, std::vector<double>(101, 0.5)});
        DAPDFSize = {1, 101};
        return true;
    }
    else
    {
        bool success=loadDAFile(DAPDFFile, DAPDF);
        DAPDFSize = {DAPDF.size(), DAPDF.begin()->second.size()};
        return success;
    }
    
}
bool NeutronCrossSection::loadDAInverseCDFFile(std::string DAInverseCDFFile)
{
    if (DAInverseCDFFile.size() == 0)
    {
        // linear distribution on [-1, 1]
        std::vector<double> invcdf(101, 0);
        for (int i = 0; i < 101; i++)
        {
            invcdf[i] = -1 + 0.02 * double(i);
        }
        DAInverseCDF.insert({1.5e7, invcdf});
        DAInvCDFSize = {1, 101};
        return true;
    }
    else
    {
        bool success=loadDAFile(DAInverseCDFFile, DAInverseCDF);
        DAInvCDFSize = {DAInverseCDF.size(), DAInverseCDF.begin()->second.size()};
        return success;
    }
}

double NeutronCrossSection::getTotalMicroscopicCrossSectionAt(double energy) const
{
    return getClosestEntry(energy, totalMicroscopicCrossSection);
}
double NeutronCrossSection::getElasticMicroscopicCrossSectionAt(double energy) const
{
    return getClosestEntry(energy, elasticMicroscopicCrossSection);
}
double NeutronCrossSection::getDAPDFAt(double energy, double mu) const
{
    const std::vector<double>& entry = getClosestEntry(energy, DAPDF);
    double mu_idx = std::floor((mu+1) / 0.02);
    // return entry[static_cast<int>(mu_idx)];

    // linear interpolation
    double xl = -1 + mu_idx * 0.02;
    double xr = xl + 0.02;
    double yl = entry[static_cast<int>(mu_idx)];
    double yr = entry[static_cast <int>(mu_idx) + 1];
    double val = (yl * (xr - mu) + yr * (mu - xl)) / 0.02;
    return val;
}
double NeutronCrossSection::getDAInvCDFAt(double energy, double probability) const
{
    const std::vector<double>& entry = getClosestEntry(energy, DAInverseCDF);
    double p_idx = std::floor(probability / 0.01);
    // return entry[static_cast<int>(p_idx)];
    
    // linear interpolation
    double xl = p_idx * 0.01;
    double xr = xl + 0.01;
    double yl = entry[static_cast<int>(p_idx)];
    double yr = entry[static_cast <int>(p_idx) + 1];
    double val = (yl * (xr - probability) + yr * (probability - xl)) / 0.01;
    return val;
}

PhotonCrossSection::PhotonCrossSection(const std::string fpath)
{
    // read attenuation coeffcients saved in txt
    std::ifstream fileptr;
    fileptr.open(fpath, std::ios::in);
    if (!fileptr.is_open())
    {
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
        totalAtten.push_back(lineData[3]);
    }
    NEbin = EBinCenters.size();
    Emin = EBinCenters[0];
    Emax = EBinCenters[NEbin - 1];
    deltaE = (Emax - Emin) / (NEbin -1);
}

double PhotonCrossSection::getAtten(const double erg) const
{
    int binNum = (erg - Emin) / deltaE + 0.5;
    if (binNum <= 0)
        return totalAtten[0];
    else if (binNum >= NEbin - 1)
        return totalAtten[NEbin - 1];
    return totalAtten[binNum];
    
}
double PhotonCrossSection::getTotalComptonIntegral(const double erg) const
{
    int binNum = (erg - Emin) / deltaE + 0.5;
    if (binNum <= 0)
        return IntegralComptonCrossSection[0];
    else if (binNum >= NEbin - 1)
        return IntegralComptonCrossSection[NEbin - 1];
    return IntegralComptonCrossSection[binNum];
}
double PhotonCrossSection::getComptonOverTotal(const double erg) const
{
    int binNum = (erg - Emin) / deltaE + 0.5;
    if (binNum <= 0)
        return ComptonOverTotal[0];
    else if (binNum >= NEbin - 1)
        return ComptonOverTotal[NEbin - 1];
    return ComptonOverTotal[binNum];
}

Material::Material(const double d, const int id, const std::vector<std::pair<double, Nuclide>>& comp)
    : density(d), matID(id), 
      compositions(comp), 
      photonCrossSection(comp[0].second.getPhotonCrossSection())
{
    molecularMass = 0;
    int densestNeutronIdx(0);
    int neutronGridNum(0);
    // int densestPhotonIdx(0);
    // int photonGridNum(0);
    for (int i=0;i<compositions.size();i++)
    {
        const auto& v = compositions[i];
        molecularMass += v.first * v.second.getAtomicWeight();
        if (neutronGridNum < v.second.getNeutronCrossSection().getTotalMicroscopicCrossSectionSize())
        {
            neutronGridNum = v.second.getNeutronCrossSection().getTotalMicroscopicCrossSectionSize();
            densestNeutronIdx = i;
        }
        // photon cross sections have the same size
    }
    std::vector<double> neutronErgGrid;
    for (auto &&v : compositions[densestNeutronIdx].second.getNeutronCrossSection().getTotalMicroscopicCrossSection())
    {
        neutronErgGrid.push_back(v.first);
    }
    micro2macro = density / molecularMass * 0.60221409; // 1e24 * cm^-3
    for (auto &&energy : neutronErgGrid)
    {
        double weightCrossSection(0); // barns, 1e-24 cm^2
        std::vector<double> invCDF{0};
        for (auto &&v : compositions)
        {
            weightCrossSection += v.first * v.second.getNeutronCrossSection().getTotalMicroscopicCrossSectionAt(energy);
            invCDF.push_back(weightCrossSection);
        }
        for (int i=0;i<invCDF.size();i++)
        {
            invCDF[i] /= weightCrossSection;
        }
        neutronTotalMicroscopicCrossSection.insert({energy, weightCrossSection});
        nuclideInvCDF.insert({energy, invCDF});
    }
}

const Nuclide& Material::selectInteractionTarget(const double energy, const double r) const
{
    const std::vector<double>& entry = getClosestEntry(energy, nuclideInvCDF);
    for (int i = 0; i < compositions.size(); i++)
    {
        if (r < entry[i+1])
        {
            return compositions[i].second;
        }
    }
}