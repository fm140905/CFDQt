#include "cfdworker.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QDebug>

CFDWorker::CFDWorker(QObject *parent)
    : QObject(parent),
      tally(Tally(Sphere(QVector3D(100, 100, 10), 2.54), 100, 0, 1.0))
{
    initSetup();
//    // initialize tally F2
//    const Sphere detector = Sphere(QVector3D(100, 100, 10), 2.54);
//    tally = Tally(detector, 100, 0, 1.0);

//    stop=false;
}

CFDWorker::~CFDWorker()
{
//    delete tally;
    delete config;
}

void CFDWorker::initSetup()
{
    // initialize gemoetry
    const Cylinder waterCylinder = Cylinder(QVector3D(25, 25, 0), 52, 21.5);
    const Cylinder sourceCylinder = Cylinder(QVector3D(25, 25, 8.4478), 5.63372, 1.4097);
    // load cross-section tables
    std::string rootdir("/media/ming/DATA/projects/2021_DTRA/cfdneutron/cfdqt");
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
    std::vector<double> srcEnergyCDF{0.661}; // Cs137
    const Source source = Source(sourceCylinder, srcEnergyCDF);
    // initialize settings
    const int maxN = 100000;
    const double maxScatterN = 5;
    const double minE = 0.1;
    const double minW = 0.01;
    config = new MCSettings(waterCylinder, std::vector<Cell>{waterCell}, source, maxN, maxScatterN, minW, minE);
}

void CFDWorker::work()
{
    QTimer timer;
    connect(&timer, &QTimer::timeout, this, &CFDWorker::getSpectrum);
    timer.start(500);
}

void CFDWorker::getSpectrum()
{
    if(!changed)
        return;

    const int maxTime=800; //ms
    QElapsedTimer timer;
    timer.start();

    for (int i=0;i<config->maxN;i++)
    {
        if(timer.hasExpired(maxTime))
        {
            tally.setNPS(i);
            changed=false;
//            qDebug() << i;
            break;
        }
        // CFD
        // create a new particle from source
        Particle prtl = config->source.createParticle();
        assert(config->ROI.contain(prtl.pos));

        // primary contribution
        forceDetection(prtl, *config, tally);

        // transport
        while (prtl.scatterN < config->maxScatterN &&
               prtl.ergE > config->minE &&
               prtl.weight > config->minW &&
               deltaTracking(prtl, *config))
        {
            prtl.scatterN += 1;
            forceDetection(prtl, *config, tally);
            ComptonScattering(prtl, *config);
        }
    }
    if(changed)
        tally.setNPS(config->maxN);
    tally.hist.scaling(1.0/config->maxN);
    emit spectrumChanged(tally);
    changed=false;
    tally.reset();
}

void CFDWorker::stopWorker()
{
    stop=true;
}

void CFDWorker::onCenterChanged(QVector3D newCenter)
{
//    qDebug() << newCenter;
    tally.setCenter(newCenter);
    changed=true;
    getSpectrum();
}
void CFDWorker::onRadiusChanged(double newradius)
{
    tally.setRadius(newradius);
    changed=true;
    getSpectrum();
}
