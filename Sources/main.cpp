#include <chrono>
#include <iostream>
#include <sys/stat.h>

#include "geometry.h"
#include "data.h"
#include "cell.h"
#include "tracking.h"
#include "cfd.h"

int main(int argc, char** argv)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    struct stat st = {0};
    // create directories to save the output files and pics, works in Linux
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }

    // initialize gemoetry
    const Cylinder waterCylinder = Cylinder(QVector3D(25, 25, 0), 52, 21.5);
    const Cylinder sourceCylinder = Cylinder(QVector3D(25, 25, 8.4478), 5.63372, 1.4097);
    // initialize material
    const double waterDensity = 0.99; // g cm^-3
    const Material water = Material("/media/ming/DATA/projects/2021_DTRA/cfdneutron/DATA/H2O.csv", waterDensity, 18);
    // initialize cell
    const Cell waterCell = Cell(water, waterDensity, waterCylinder);
    // initialize source
    Histogram gammaCDF = Histogram(1, 0, 0.661*2);
    gammaCDF.setBinContents(std::vector<double>{1});
    const Source gammaSource = Source(sourceCylinder, gammaCDF);
    // initialize settings
    const int maxN = 1000000;
    const double maxScatterN = 5;
    const double minE = 0.1;
    const double minW = 0.01;
    const MCSettings config = MCSettings(waterCylinder, std::vector<Cell>{waterCell}, gammaSource, maxN, maxScatterN, minW, minE);
    // initialize tally F2
    const Sphere detector = Sphere(QVector3D(100, 100, 10), 2.54);
    Tally tally = Tally(detector, 100, 0, 1.0);

    Tally energy = Tally(detector, 100, 0, 1.0);

    // run photon transport and CFD
    // std::vector<Particle> scatterParticles;
    for (int i = 0; i < config.maxN; i++)
    {
        // create a new particle from source
        Particle prtl = config.source.createParticle();
        assert(config.ROI.contain(prtl.pos));

        // primary contribution
        forceDetection(prtl, config, tally);

        // transport
        while (prtl.scatterN < config.maxScatterN &&
               prtl.ergE > config.minE &&
               prtl.weight > config.minW &&
               deltaTracking(prtl, config))
        {
            prtl.scatterN += 1;
            forceDetection(prtl, config, tally);
            ComptonScattering(prtl, config);
            // if (prtl.scatterN == 1)
            // {
            //     energy.Fill(prtl);
            // }
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
    for (std::size_t i = 0; i < tally.hist.getNBins(); i++)
    {
        fileptr << tally.hist.getBinCenter(i) << '\t' << tally.hist.getBinContent(i) / config.maxN << '\n';
    }
    fileptr.close();
    
    return 0;
}