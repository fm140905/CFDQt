/**
 * @file neutron.cpp
 * @author Ming Fang
 * @brief Simulate the spectrum of a Cf source in a water barrel.
 *        Thermal treatment is included.
 * @version 0.1
 * @date 2022-08-04
 * 
 */
#include <chrono>
#include <iostream>
#include <sys/stat.h>
#include <cassert>

#include "geometry.h"
#include "data.h"
#include "material.h"
#include "cell.h"
#include "tracking.h"
#include "cfd.h"

int main(int argc, char** argv)
{
    // create directories to save the output files and pics
    std::filesystem::create_directories("output_neutron");
    std::filesystem::path cwd(std::filesystem::current_path());
    std::string rootdir = cwd.parent_path().string();
    // initialize gemoetry
    const Cylinder waterCylinder = Cylinder(Vector3D(25, 25, 0), 52, 5);
    const Cylinder sourceCylinder = Cylinder(Vector3D(25, 25, 8.4478), 5.63372, 1.4097);
    // load cross-section tables
    const PhotonCrossSection photonCrossSection(rootdir+"/DATA/H2O.csv");
    const NeutronCrossSection H1NeutronCrossSection(rootdir+"/DATA/H1-total-cross-section.txt",
                                                    rootdir+"/DATA/H1-elastic-scattering-cross-section.txt");
    const NeutronCrossSection O16NeutronCrossSection(rootdir+"/DATA/O16-total-cross-section.txt",
                                                     rootdir+"/DATA/O16-elastic-scattering-cross-section.txt",
                                                     rootdir+"/DATA/O16-elastic-scattering-PDF.txt",
                                                     rootdir+"/DATA/O16-elastic-scattering-CDF.txt");
    // create nuclides
    const Nuclide H1(1, 1, H1NeutronCrossSection, photonCrossSection);
    const Nuclide O16(8, 16, O16NeutronCrossSection, photonCrossSection);
    // initialize material
    const double waterDensity = 0.99; // g cm^-3
    const Material water = Material(waterDensity, 18, {{2, H1}, {1, O16}});
    // initialize cell
    const Cell waterCell = Cell(water, waterDensity, waterCylinder);
    // initialize source, eV for neutron, MeV for gamma
    std::vector<double> srcEnergyCDF{0, 478702, 817756, 1.15082e6,
                                    1.50133e6,1.88769e6,2.33337e6,2.87784e6,
                                    3.60501e6,4.77086e6,1e7}; // Cf-252
    const Source source = Source(sourceCylinder, srcEnergyCDF);
    // initialize settings
    const int maxN = 1000000;
    const double maxScatterN = 100;
    const double minE = 1e-4;  // eV for neutron, MeV for gamma
    const double minW = 0.01;
    const MCSettings config = MCSettings(waterCylinder, std::vector<Cell>{waterCell}, source, maxN, maxScatterN, minW, minE);
    // initialize tally F2
    const Sphere detector = Sphere(Vector3D(75, 75, 10), 2.54);
    // lethargy
    Tally tally = Tally(detector, 110, 1e-3, 1e8, true);

    // run neutron transport and CFD
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < config.maxN; i++)
    {
        // if(i % 1000 == 0)
        // {
        //     std::cout << i << '\n';
        // }
        // create a new particle from source
        Particle prtl = config.source.createParticle();
        assert(config.ROI.contain(prtl.pos));

        // primary contribution
        forceDetectionNeutron(prtl, config, tally);

        // transport
        while (prtl.scatterN < config.maxScatterN &&
               prtl.ergE > config.minE &&
               prtl.weight > config.minW &&
               deltaTrackingNeutron(prtl, config))
        {
            prtl.scatterN += 1;
            forceDetectionNeutron(prtl, config, tally);
            neutronElasticScattering(prtl, config);
        }
    }


    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;

    std::ofstream fileptr;
    std::string fpath = "output_neutron/tally.txt";
    fileptr.open(fpath, std::ios::out);
    if (!fileptr.is_open())
    {
        std::string errMessage = "can't open file: " + fpath;
        throw std::runtime_error(errMessage);
    }
    for (std::size_t i = 0; i < tally.getNBins(); i++)
    {
        fileptr << tally.getBinCenter(i) << '\t' << tally.getBinContent(i) / config.maxN << '\n';
    }
    fileptr.close();
    
    return 0;
}