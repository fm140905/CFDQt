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
    const Material water = Material("DATA/H2O.csv", 18);
    const double waterDensity = 0.99; // g cm^-3
    // initialize cell
    const Cell waterCell = Cell(water, waterDensity, waterCylinder);
    // initialize source
    Histogram gammaCDF = Histogram(1, 0, 0.661*2);
    gammaCDF.setBinContents(std::vector<double>{1});
    const Source gammaSource = Source(sourceCylinder, gammaCDF);
    // initialize settings
    const int maxN = 1000;
    const double maxScatterN = 5;
    const double minE = 0.1;
    const double minW = 0.01;
    const MCSettings config = MCSettings(waterCylinder, gammaSource, maxN, maxScatterN, minW, minE);
    // initialize tally F2
    const Sphere detector = Sphere(QVector3D(100, 100, 10), 2.54);
    Tally tally = Tally(detector, 100, 0, 1.0);

    // run photon transport and CFD
    // TODO

    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;
    return 0;
}