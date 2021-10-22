/**
 * @file material.h
 * @author your name (you@domain.com)
 * @brief Generate cross-section tables
 * @version 0.1
 * @date 2021-10-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <math.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

template< class T >
const T& getClosestEntry(const double key, const std::map<double, T>& m)
{
    auto lb = m.lower_bound(key);
    if (lb == m.end())
    {
        // nothing found, energy out of range
        // return the last val
        return m.rbegin()->second;
    }
    else if (lb == m.begin())
    {
        return lb->second;
    }
    else
    {
        auto prev = std::prev(lb);
        if ((key-prev->first) < (lb->first - key))
        {
            return prev->second;
        }
        else
        {
            return lb->second;
        }
    }
}

class NeutronCrossSection
{
private:
    /* data */
    int atomicNumber;
    int massNumber;
    std::map<double, double> totalMicroscopicCrossSection;
    std::map<double, double> elasticMicroscopicCrossSection;
    // PDF of elastic scattering angular distribution, in CMS
    // key = energy, val = PDF of N + 1 points corresponding to N equal-distributed mu bins
    // N = 100 => mu = -1.0, -0.98, ..., 0.98, 1.0
    std::map<double, std::vector<double>> DAPDF;
    // Inverse CDF of elastic scattering angular distribution, in CMS
    // key = energy, val = N + 1 points corresponding to N equal-probable bins
    // N = 100 => CDF = 0, 0.01, ..., 0.99, 1.0
    std::map<double, std::vector<double>> DAInverseCDF;

    // load files
    bool loadCrossSection(std::string crossSectionFile, std::map<double, double> crossSectionTable);

    bool loadDAFile(std::string DAFile, std::map<double, std::vector<double>> DATable);
    bool loadTotalCrossSection(std::string totalCrossSectionFile);
    bool loadElasticCrossSection(std::string elasticCrossSectionFile);
    bool loadDAPDFFile(std::string DAPDFFile);
    bool loadDAInverseCDFFile(std::string DAInverseCDFFile);

public:
    NeutronCrossSection(int z, int a, 
                 std::string totalCrossSectionFile, 
                 std::string elasticCrossSectionFile)
                : NeutronCrossSection(z, a, totalCrossSectionFile, elasticCrossSectionFile, "", "") {}
    
    NeutronCrossSection(int z, int a, 
                 std::string totalCrossSectionFile, 
                 std::string elasticCrossSectionFile, 
                 std::string DAPDFFile, 
                 std::string DAInverseCDFFile)
                : atomicNumber(z), massNumber(a)
                {
                    loadTotalCrossSection(totalCrossSectionFile);
                    loadElasticCrossSection(elasticCrossSectionFile);
                    loadDAPDFFile(DAPDFFile);
                    loadDAInverseCDFFile(DAInverseCDFFile);
                }
    const double& getTotalMicroScopicCrossSectionAt(double energy);
    const double& getElasticMicroScopicCrossSectionAt(double energy);
    const double& getDAPDFAt(double energy, double mu);
    const double& getDAInvCDFAt(double energy, double probability);
};
