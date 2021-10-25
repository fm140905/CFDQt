#include <gtest/gtest.h>
#include "cell.h"

TEST(ParticleTest, constructor)
{
    const QVector3D p = QVector3D(0, 0, 0);
    const QVector3D d = QVector3D(1, 1, 1);
    double initE = 0.6617;
    Particle prtl = Particle(p, d, initE, 1.0);

    EXPECT_DOUBLE_EQ(prtl.ergE, initE);
    EXPECT_DOUBLE_EQ(prtl.weight, 1.0);
    EXPECT_EQ(prtl.scatterN, 0);
    EXPECT_FALSE(prtl.escaped);
    EXPECT_EQ(prtl.pos, QVector3D(0, 0, 0));
    EXPECT_NEAR(prtl.dir.x(), 1/qSqrt(3), 1e-5);
    EXPECT_NEAR(prtl.dir.y(), 1/qSqrt(3), 1e-5);
    EXPECT_NEAR(prtl.dir.z(), 1/qSqrt(3), 1e-5);

    prtl.move(qSqrt(3));
    EXPECT_NEAR(prtl.pos.x(), 1, 1e-5);
    EXPECT_NEAR(prtl.pos.y(), 1, 1e-5);
    EXPECT_NEAR(prtl.pos.z(), 1, 1e-5);
}

TEST(CellTest, contain)
{
    std::string rootdir = "/home/mingf2/projects/2021_DTRA/";
    // initialize gemoetry
    const Cylinder waterCylinder = Cylinder(QVector3D(25, 25, 0), 52, 21.5);
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
    // initialize cell
    const Cell waterCell = Cell(water, waterDensity, waterCylinder);

    Particle prtl = Particle(QVector3D(10, 10, 20), QVector3D(1, 1, 1), 0.6617, 1.0);
    EXPECT_TRUE(waterCell.contains(prtl));

    prtl = Particle(QVector3D(0, 0, 20), QVector3D(1, 1, 1), 0.6617, 1.0);
    EXPECT_FALSE(waterCell.contains(prtl));
}

TEST(SourceTest, createMonoenergeticParticle)
{
    // initialize source
    std::vector<double> srcEnergyCDF{0.661}; // eV for neutron, MeV for gamma
    const Cylinder sourceCylinder = Cylinder(QVector3D(25, 25, 8.4478), 5.63372, 1.4097);
    const Source source = Source(sourceCylinder, srcEnergyCDF);

    Particle prtl = source.createParticle();
    EXPECT_DOUBLE_EQ(prtl.ergE, 0.661);
    EXPECT_TRUE(sourceCylinder.contain(prtl.pos));
    EXPECT_DOUBLE_EQ(prtl.dir.length(), 1.0);
}

TEST(SourceTest, createDistributedParticle)
{
    // initialize source
    std::vector<double> srcEnergyCDF{0, 0.478702, 0.817756, 1.15082,
                                    1.50133,1.88769,2.33337,2.87784,
                                    3.60501,4.77086,10.0}; // eV for neutron, MeV for gamma
    const Cylinder sourceCylinder = Cylinder(QVector3D(25, 25, 8.4478), 5.63372, 1.4097);
    const Source source = Source(sourceCylinder, srcEnergyCDF);

    std::ofstream fileptr;
    std::string fpath = "energy_list.txt";
    fileptr.open(fpath, std::ios::out);
    if (!fileptr.is_open())
    {
        std::string errMessage = "can't open file: " + fpath;
        throw std::runtime_error(errMessage);
    }
    for (std::size_t i = 0; i < 100000; i++)
    {
        fileptr << source.createParticle().ergE << '\n';
    }
    fileptr.close();
}