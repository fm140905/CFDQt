#include "data.h"

Material::Material(const std::string fpath, const int id)
    : matID(id)
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
    // Read one line at a time into the variable line:
    while(std::getline(fileptr, line))
    {
        std::vector<float> lineData;
        std::stringstream lineStream(line);

        float value;
        // Read an float at a time from the line
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

float Material::getAtten(const double erg) const
{
    int binNum = (erg - Emin) / deltaE + 0.5;
    if (binNum <= 0)
        return totalAtten[0];
    else if (binNum >= NEbin - 1)
        return totalAtten[NEbin - 1];
    return totalAtten[binNum];
    
}
float Material::getTotalComptonIntegral(const double erg) const
{
    int binNum = (erg - Emin) / deltaE + 0.5;
    if (binNum <= 0)
        return IntegralComptonCrossSection[0];
    else if (binNum >= NEbin - 1)
        return IntegralComptonCrossSection[NEbin - 1];
    return IntegralComptonCrossSection[binNum];
    
}
float Material::getComptonOverTotal(const double erg) const
{
    int binNum = (erg - Emin) / deltaE + 0.5;
    if (binNum <= 0)
        return ComptonOverTotal[0];
    else if (binNum >= NEbin - 1)
        return ComptonOverTotal[NEbin - 1];
    return ComptonOverTotal[binNum];
}