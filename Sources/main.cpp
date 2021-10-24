#include <chrono>
#include <iostream>
#include <sys/stat.h>

#include "geometry.h"
#include "data.h"
#include "material.h"
#include "cell.h"
#include "tracking.h"
#include "cfd.h"

int main(int argc, char** argv)
{
    struct stat st = {0};
    // create directories to save the output files and pics, works in Linux
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }
    std::string rootdir = "/home/mingf2/projects/2021_DTRA/";
    // initialize gemoetry
    const Cylinder waterCylinder = Cylinder(QVector3D(25, 25, 0), 52, 5);
    const Cylinder sourceCylinder = Cylinder(QVector3D(25, 25, 8.4478), 5.63372, 1.4097);
    // load cross-section tables
    const PhotonCrossSection photonCrossSection(rootdir+"DATA/H2O.csv");
    const NeutronCrossSection H1NeutronCrossSection(rootdir+"DATA/H1-total-cross-section.txt",
                                                    rootdir+"DATA/H1-elastic-scattering-cross-section.txt");
    const NeutronCrossSection O16NeutronCrossSection(rootdir+"DATA/O16-total-cross-section.txt",
                                                     rootdir+"DATA/O16-elastic-scattering-cross-section.txt",
                                                     rootdir+"DATA/O16-elastic-scattering-PDF.txt",
                                                     rootdir+"DATA/O16-elastic-scattering-CDF.txt");
    // create nuclides
    const Nuclide H1(1, 1, H1NeutronCrossSection, photonCrossSection);
    const Nuclide O16(8, 16, O16NeutronCrossSection, photonCrossSection);
    // initialize material
    const double waterDensity = 0.99; // g cm^-3
    const Material water = Material(waterDensity, 18, {{2, H1}, {1, O16}});
    // const double waterDensity = 0.11; // g cm^-3
    // const Material water = Material(waterDensity, 18, {{1, H1}});
    // initialize cell
    const Cell waterCell = Cell(water, waterDensity, waterCylinder);
    // initialize source
    Histogram srcEnergyCDF = Histogram(1, 0, 999999*2); // eV for neutron, MeV for gamma
    srcEnergyCDF.setBinContents(std::vector<double>{1});
    const Source gammaSource = Source(sourceCylinder, srcEnergyCDF);
    // initialize settings
    const int maxN = 100000;
    const double maxScatterN = 100;
    const double minE = 1;  // eV for neutron, MeV for gamma
    const double minW = 0.01;
    const MCSettings config = MCSettings(waterCylinder, std::vector<Cell>{waterCell}, gammaSource, maxN, maxScatterN, minW, minE);
    // initialize tally F2
    const Sphere detector = Sphere(QVector3D(75, 75, 10), 2.54);
    // // linear
    // Tally tally = Tally(detector, 100, 1e-4, 1e6);
    // lethargy
    Tally tally = Tally(detector, 110, 1e-4, 1e7, true);

    // Tally energy = Tally(detector, 100, 0, 1e6);

    // run neutron transport and CFD
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < config.maxN; i++)
    {
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
    std::string fpath = "output/tally.txt";
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