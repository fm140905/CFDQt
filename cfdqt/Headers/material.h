/**
 * @file material.h
 * @author Ming Fang
 * @brief Generate cross-section tables
 * @version 0.1
 * @date 2021-10-22
 * 
 * @author Ming Fang
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

/**
 * @brief Get the map entry whose key is the closest to the given key.
 * 
 * @tparam T 
 * @param key Entry key to be checked
 * @param m Map
 * @return const T& . Map entry whose key is the closest to the given key.
 */
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

/**
 * @brief Neutron cross section of a nuclide.
 * 
 */
class NeutronCrossSection
{
private:
    // total microscopic neutron cross-section, barns, as a function of energy
    std::map<double, double> totalMicroscopicCrossSection;
    // neutron elastic scattering cross-section, barns, as a function of energy
    std::map<double, double> elasticMicroscopicCrossSection;
    // PDF of elastic scattering angular distribution, in CMS, as a function of energy
    // key = energy, val = PDF of N + 1 points corresponding to N equal-distributed mu bins
    // N = 100 => mu = -1.0, -0.98, ..., 0.98, 1.0
    std::map<double, std::vector<double>> DAPDF;
    std::pair<int, int> DAPDFSize;
    // Inverse CDF of elastic scattering angular distribution, in CMS, as a function of energy
    // key = energy, val = N + 1 points corresponding to N equal-probable bins
    // N = 100 => CDF = 0, 0.01, ..., 0.99, 1.0
    std::map<double, std::vector<double>> DAInverseCDF;
    std::pair<int, int> DAInvCDFSize;

    /**
     * @brief Load cross-section from file
     * 
     * @param crossSectionFile 
     * @param crossSectionTable 
     * @return true 
     * @return false 
     */
    bool loadCrossSection(std::string crossSectionFile, std::map<double, double>& crossSectionTable);
    bool loadDAFile(std::string DAFile, std::map<double, std::vector<double>>& DATable);
    bool loadTotalCrossSection(std::string totalCrossSectionFile);
    bool loadElasticCrossSection(std::string elasticCrossSectionFile);
    bool loadDAPDFFile(std::string DAPDFFile);
    bool loadDAInverseCDFFile(std::string DAInverseCDFFile);

public:
    /**
     * @brief Construct a new Neutron Cross Section object. Neutron elastic scattering is isotropic.
     * 
     * @param totalCrossSectionFile File that records the total neutron cross section
     * @param elasticCrossSectionFile File that records the neutron elastic scattering cross section
     */
    NeutronCrossSection(
                 std::string totalCrossSectionFile, 
                 std::string elasticCrossSectionFile)
                : NeutronCrossSection(totalCrossSectionFile, elasticCrossSectionFile, "", "") {}
    
    /**
     * @brief Construct a new Neutron Cross Section object
     * 
     * @param totalCrossSectionFile File that records the total neutron cross section
     * @param elasticCrossSectionFile File that records the neutron elastic scattering cross section
     * @param DAPDFFile File that records the PDF of neutron elastic scattering angular distribution
     * @param DAInverseCDFFile File that records inverse of CDF of the neutron elastic scattering angular distribution
     */
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
    /**
     * @brief Get the Total Microscopic Cross Section At a given energy
     * 
     * @param energy 
     * @return double 
     */
    double getTotalMicroscopicCrossSectionAt(double energy) const;
    /**
     * @brief Get the number of Total Microscopic Cross Section tables
     * 
     * @return int 
     */
    int getTotalMicroscopicCrossSectionSize() const {return totalMicroscopicCrossSection.size();}
    const std::map<double, double>& getTotalMicroscopicCrossSection() const {return totalMicroscopicCrossSection;}
    /**
     * @brief Get the Elastic Microscopic Cross Section At a given energy
     * 
     * @param energy 
     * @return double 
     */
    double getElasticMicroscopicCrossSectionAt(double energy) const;
    /**
     * @brief Get the number of Elastic Microscopic Cross Section Size tables
     * 
     * @return int 
     */
    int getElasticMicroscopicCrossSectionSize() const {return elasticMicroscopicCrossSection.size();}
    /**
     * @brief Get the probability density for a neutron of the given energy to be scattered with the given scattering angle 
     * 
     * @param energy Neutron energy
     * @param mu Cosine of neutron scattering angle
     * @return double 
     */
    double getDAPDFAt(double energy, double mu) const;
    std::pair<int, int> getDAPDFSize() const {return DAPDFSize;}
    /**
     * @brief Get the neutron scattering angle theta that satifies CDF(cos(theta)) = probability
     * 
     * @param energy Neutron energy
     * @param probability 
     * @return double Cosine of the scattering angle
     */
    double getDAInvCDFAt(double energy, double probability) const;
    std::pair<int, int> getDAInvCDFSize() const {return DAInvCDFSize;}
};

/**
 * @brief Photon cross section of a material.
 * 
 */
class PhotonCrossSection
{
private:
    // center of energy bins, MeV
    std::vector<double> EBinCenters;
    // integrated Compton scattering cross section over 4pi solid angle
    std::vector<double> IntegralComptonCrossSection;
    // ratio of Compton scattering cross section and total cross section
    std::vector<double> ComptonOverTotal;
    // total attenuation coefficient, mu/rho, cm^2/g
    std::vector<double> totalAtten;
    // Left edge of first energy bin
    double Emin;
    // Right edge of last energy bin
    double Emax;
    // Energy bin width
    double deltaE;
    // Number of energy bins
    int NEbin;
public:
    /**
     * @brief Construct a new Photon Cross Section object
     * 
     * @param fpath File that records the photon cross section.
     */
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
    double molecularMass; // g/mol
    // fractions of each nuclide
    std::vector<std::pair<double, Nuclide>> compositions;
    std::map<double, std::vector<double>> nuclideInvCDF;
    double micro2macro; // macro = micro * rho / M * NA, barn -> cm^-1
    std::map<double, double> neutronTotalMicroscopicCrossSection; // barn
    const PhotonCrossSection photonCrossSection;
    // std::map<double, double> photonTotalMacroscopicCrossSection; // cm^{-1}
public:
    /**
     * @brief Construct a new Material object
     * 
     * @param d Material density
     * @param id Material ID
     * @param comp Nuclide composition
     */
    Material(const double d, const int id, const std::vector<std::pair<double, Nuclide>>& comp);

    double getDensity() const {return density;}
    /**
     * @brief Get macroscopic total neutron cross section at the given energy, cm^-1
     * 
     * @param energy 
     * @return double 
     */
    double getNeutronTotalAtten(double energy) const {return micro2macro * getNeutronTotalMicroscopicCrossSection(energy);}
    /**
     * @brief Get microscopic total neutron cross section, cm^-1
     * 
     * @param energy 
     * @return double 
     */
    double getNeutronTotalMicroscopicCrossSection(double energy) const {return getClosestEntry(energy, neutronTotalMicroscopicCrossSection);}
    // double getPhotonTotalAtten(double energy) const {return getClosestEntry(energy, photonTotalMacroscopicCrossSection);}
    /**
     * @brief Get macroscopic total photon cross section at the given energy, cm^-1
     * 
     * @param energy 
     * @return double 
     */
    double getPhotonTotalAtten(double energy) const {return density * photonCrossSection.getAtten(energy);}
    const PhotonCrossSection& getPhotonCrossSection() const {return photonCrossSection;}
    const std::vector<std::pair<double, Nuclide>>& getNuclideComposition() const {return compositions;}
    int getNumberOfNuclides() const {return compositions.size();}
    /**
     * @brief Given random number r, sample the nuclide that the neutron is going to interact with.
     * 
     * @param energy Neutron energy
     * @param r A random number
     * @return const Nuclide& 
     */
    const Nuclide& selectInteractionTarget(const double energy, const double r) const;
};