#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <matplot/matplot.h>

bool readMCNPResult(std::string filepath, int startLine, int endLine,
              std::vector<double>& ergBins, 
              std::vector<double>& counts, 
              std::vector<double>& errors)
{
    std::ifstream fileptr;
    fileptr.open(filepath, std::ios::in);
    if (!fileptr.is_open())
    {
        std::string errMessage = "can't open file: " + filepath;
        throw std::runtime_error(errMessage);
    }
    std::string line;
    int lineNum(1);
    // Read one line at a time into the variable line:
    while(std::getline(fileptr, line))
    {
        if (lineNum < startLine)
        {
            lineNum += 1;
            continue;
        }
        if (lineNum > endLine)
        {
            break;
        }
        lineNum++;
        std::stringstream lineStream(line);
        double energy, value, uncertainty;
        // std::string s;
        while(lineStream >> energy >> value >> uncertainty)
        {
            ergBins.push_back(energy);
            counts.push_back(value);
            errors.push_back(uncertainty);
        }
    }
    return true;
}

bool readCFDResult(std::string filepath, 
                   std::vector<double>& ergBins, 
                   std::vector<double>& counts)
{
    std::ifstream fileptr;
    fileptr.open(filepath, std::ios::in);
    if (!fileptr.is_open())
    {
        std::string errMessage = "can't open file: " + filepath;
        throw std::runtime_error(errMessage);
    }
    std::string line;
    // Read one line at a time into the variable line:
    while(std::getline(fileptr, line))
    {
        std::stringstream lineStream(line);
        double energy, value;
        // std::string s;
        while(lineStream >> energy >> value)
        {
            ergBins.push_back(energy);
            counts.push_back(value);
        }
    }
    return true;
}


int main(int argc, char** argv)
{
    std::string mcnpResultPath;
    std::string CFDResultPath;
    int startLine;
    int endLine;
    std::vector<double> ergBinsMCNP;
    std::vector<double> countsMCNP;
    std::vector<double> errorsMCNP;
    std::vector<double> ergBinsCFD;
    std::vector<double> countsCFD;
    if (argc < 2)
    {
        std::cerr << "Please provide an input: neutron, or gamma " << std::endl;
        exit(1);
    }
    else if(std::string(argv[1]) == "neutron")
    {
        mcnpResultPath = "output_neutron/outq";
        startLine = 510;
        endLine = 610;
        CFDResultPath = "output_neutron/tally.txt";
        readMCNPResult(mcnpResultPath, startLine, endLine, ergBinsMCNP, countsMCNP, errorsMCNP);
        readCFDResult(CFDResultPath, ergBinsCFD, countsCFD);
        std::vector<double> d_lethargy;
        for (int i = 0; i < ergBinsMCNP.size(); i++)
        {
            ergBinsMCNP[i] *= 1000;
        }
        for (int i = 0; i < ergBinsMCNP.size() - 1; i++)
        {
            d_lethargy.push_back(std::log(ergBinsMCNP[i+1]) - std::log(ergBinsMCNP[i]));
        }
        ergBinsMCNP.erase(ergBinsMCNP.begin());
        countsMCNP.erase(countsMCNP.begin());
        for (int i = 0; i < ergBinsMCNP.size(); i++)
        {
            countsMCNP[i] /= d_lethargy[i];
            countsMCNP[i] *= 1000;
        }
        for (int i = 0; i < ergBinsMCNP.size(); i++)
        {
            countsCFD[i] /= d_lethargy[i];
            countsCFD[i] *= 1000;
        }
        countsCFD.erase(countsCFD.begin()+ergBinsMCNP.size(), countsCFD.end());
        
        auto h = matplot::loglog(ergBinsMCNP, countsMCNP, ergBinsMCNP, countsCFD);
        // matplot::axis({1e-9, 10, 1e-7, 4e-4});
        h[0]->marker(matplot::line_spec::marker_style::circle).marker_size(4);
        h[1]->marker(matplot::line_spec::marker_style::circle).marker_face_color("m");
        matplot::legend({"MCNP, NPS = 1E8", "CFD, NPS = 1E6"})->location(matplot::legend::general_alignment::topleft);
        matplot::xlabel("Energy (keV)");
        matplot::ylabel("Fluence per NPS per lethargy (1E-3)");
    }
    else if (std::string(argv[1]) == "gamma")
    {
        mcnpResultPath = "output_gamma/outq";
        startLine = 622;
        endLine = 721;
        CFDResultPath = "output_gamma/tally.txt";
        readMCNPResult(mcnpResultPath, startLine, endLine, ergBinsMCNP, countsMCNP, errorsMCNP);
        readCFDResult(CFDResultPath, ergBinsCFD, countsCFD);

        for (int i = 0; i < ergBinsCFD.size(); i++)
        {
            if (ergBinsCFD[i] <= 0.1)
            {
                countsCFD[i] = 0;
            }
            countsCFD[i] *= 1000;
        }

        for (int i = 0; i < ergBinsMCNP.size(); i++)
        {
            ergBinsMCNP[i] -= 0.005;
            if (ergBinsMCNP[i] <= 0.1)
            {
                countsMCNP[i] = 0;
            }
            countsMCNP[i] *= 1000;
        }

        auto h = matplot::semilogy(ergBinsMCNP, countsMCNP, ergBinsMCNP, countsCFD);
        // matplot::axis({1e-9, 10, 1e-7, 4e-4});
        h[0]->marker(matplot::line_spec::marker_style::circle).marker_size(4);
        h[1]->marker(matplot::line_spec::marker_style::circle).marker_face_color("m");
        matplot::legend({"MCNP, NPS = 1E8", "CFD, NPS = 1E7"})->location(matplot::legend::general_alignment::topleft);
        matplot::xlabel("Energy (keV)");
        matplot::ylabel("Fluence per NPS (1E-3)");
        matplot::axis({0,0.7,0.001,0.1});
    }
    else
    {
        std::cerr << "Invalid input: " << argv[1] << std::endl;
        exit(1);
    }

    // // plot
    // std::vector<std::vector<double>> X{ergBinsMCNP, ergBinsCFD};
    // std::vector<std::vector<double>> Y{countsMCNP, countsCFD};

    
    matplot::show();
    return 0;
}