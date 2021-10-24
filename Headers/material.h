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
#include <utility>

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
        // linear 
    }
}

class NeutronCrossSection
{
private:
    /* data */
    std::map<double, double> totalMicroscopicCrossSection;
    std::map<double, double> elasticMicroscopicCrossSection;
    // PDF of elastic scattering angular distribution, in CMS
    // key = energy, val = PDF of N + 1 points corresponding to N equal-distributed mu bins
    // N = 100 => mu = -1.0, -0.98, ..., 0.98, 1.0
    std::map<double, std::vector<double>> DAPDF;
    std::pair<int, int> DAPDFSize;
    // Inverse CDF of elastic scattering angular distribution, in CMS
    // key = energy, val = N + 1 points corresponding to N equal-probable bins
    // N = 100 => CDF = 0, 0.01, ..., 0.99, 1.0
    std::map<double, std::vector<double>> DAInverseCDF;
    std::pair<int, int> DAInvCDFSize;

    // load files
    bool loadCrossSection(std::string crossSectionFile, std::map<double, double>& crossSectionTable);

    bool loadDAFile(std::string DAFile, std::map<double, std::vector<double>>& DATable);
    bool loadTotalCrossSection(std::string totalCrossSectionFile);
    bool loadElasticCrossSection(std::string elasticCrossSectionFile);
    bool loadDAPDFFile(std::string DAPDFFile);
    bool loadDAInverseCDFFile(std::string DAInverseCDFFile);

public:
    // NeutronCrossSection() = default;
    NeutronCrossSection(
                 std::string totalCrossSectionFile, 
                 std::string elasticCrossSectionFile)
                : NeutronCrossSection(totalCrossSectionFile, elasticCrossSectionFile, "", "") {}
    
    NeutronCrossSection(
                 std::string totalCrossSectionFile, 
                 std::string elasticCrossSectionFile, 
                 std::string DAPDFFile, 
                 std::string DAInverseCDFFile)
                {
                    loadTotalCrossSection(totalCrossSectionFile);
                    loadElasticCrossSection(elasticCrossSectionFile);
                    loadDAPDFFile(DAPDFFile);
                    loadDAInverseCDFFile(DAInverseCDFFile);
                }
    double getTotalMicroScopicCrossSectionAt(double energy) const;
    int getTotalMicroScopicCrossSectionSize() const {return totalMicroscopicCrossSection.size();}
    const std::map<double, double>& getTotalMicroScopicCrossSection() const {return totalMicroscopicCrossSection;}
    double getElasticMicroScopicCrossSectionAt(double energy) const;
    int getElasticMicroScopicCrossSectionSize() const {return elasticMicroscopicCrossSection.size();}
    double getDAPDFAt(double energy, double mu) const;
    std::pair<int, int> getDAPDFSize() const {return DAPDFSize;}
    double getDAInvCDFAt(double energy, double probability) const;
    std::pair<int, int> getDAInvCDFSize() const {return DAInvCDFSize;}
};

class PhotonCrossSection
{
private:
    // center of energy bins, MeV
    std::vector<double> EBinCenters;
    // integrated Compton cross section over 4pi slod angle
    std::vector<double> IntegralComptonCrossSection;
    // Compton over total
    std::vector<double> ComptonOverTotal;
    // total attenuation coefficient, mu/rho, cm^2/g
    std::vector<double> totalAtten;
    // min energy
    double Emin;
    double Emax;
    double deltaE;
    int NEbin;
public:
    // PhotonCrossSection() = default;
    PhotonCrossSection(const std::string fpath);
    
    double getMaxE() const {return Emax;}
    double getMinE() const {return Emin;}
    double getBinWidth() const {return deltaE;}
    double getEBinCenter(const int i) const {return EBinCenters[i];}
    double getNbins() const {return NEbin;}
    double getAtten(const double erg) const;
    double getTotalComptonIntegral(const double erg) const;
    double getComptonOverTotal(const double erg) const;
};

class Nuclide
{
private:
    /* data */
    const int atomicNumber;
    const double atomicWeight;
    const NeutronCrossSection neutronCrossSection;
    const PhotonCrossSection photonCrossSection;
public:
    // Nuclide() = default;
    Nuclide(int z, double a, const NeutronCrossSection ncs, const PhotonCrossSection pcs)
        : atomicNumber(z), atomicWeight(a),
          neutronCrossSection(ncs),
          photonCrossSection(pcs)
        {
        }
    const NeutronCrossSection& getNeutronCrossSection() const {return neutronCrossSection;}
    const PhotonCrossSection& getPhotonCrossSection() const {return photonCrossSection;}
    int getAtomicNumber() const {return atomicNumber;}
    double getAtomicWeight() const {return atomicWeight;}
};

class Material
{
private:
    int matID;
    double density; // g/cc
    double molecularMass;
    std::vector<std::pair<double, Nuclide>> compositions;
    std::map<double, std::vector<double>> nuclideInvCDF;
    double micro2macro; // macro = micro * rho / M * NA, barn -> cm^-1
    std::map<double, double> neutronTotalMicroscopicCrossSection; // barn
    const PhotonCrossSection photonCrossSection;
    // std::map<double, double> photonTotalMacroscopicCrossSection; // cm^{-1}
public:
    // constructor
    // Material() = default;
    Material(const double d, const int id, const std::vector<std::pair<double, Nuclide>>& comp);

    double getDensity() const {return density;}
    double getNeutronTotalAtten(double energy) const {return micro2macro * getNeutronTotalMicroscopicCrossSection(energy);}
    double getNeutronTotalMicroscopicCrossSection(double energy) const {return getClosestEntry(energy, neutronTotalMicroscopicCrossSection);}
    // double getPhotonTotalAtten(double energy) const {return getClosestEntry(energy, photonTotalMacroscopicCrossSection);}
    double getPhotonTotalAtten(double energy) const {return density * photonCrossSection.getAtten(energy);}
    const PhotonCrossSection& getPhotonCrossSection() const {return photonCrossSection;}
    const std::vector<std::pair<double, Nuclide>>& getNuclides() const {return compositions;}
    int getNuclidesNumber() const {return compositions.size();}
    const Nuclide& selectInteractionTarget(const double energy, const double r) const;
};