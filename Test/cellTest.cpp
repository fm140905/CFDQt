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
    // initialize gemoetry
    const Cylinder waterCylinder = Cylinder(QVector3D(25, 25, 0), 52, 21.5);
    const Cylinder sourceCylinder = Cylinder(QVector3D(25, 25, 8.4478), 5.63372, 1.4097);
    // initialize material
    const double waterDensity = 0.99; // g cm^-3
    const Material water = Material("/media/ming/DATA/projects/2021_DTRA/cfdGamma/DATA/H2O.csv", waterDensity, 18);
    // initialize cell
    const Cell waterCell = Cell(water, waterDensity, waterCylinder);

    Particle prtl = Particle(QVector3D(10, 10, 20), QVector3D(1, 1, 1), 0.6617, 1.0);
    EXPECT_TRUE(waterCell.contains(prtl));

    prtl = Particle(QVector3D(0, 0, 20), QVector3D(1, 1, 1), 0.6617, 1.0);
    EXPECT_FALSE(waterCell.contains(prtl));
}

TEST(SourceTest, createParticle)
{
    // initialize source
    Histogram gammaCDF = Histogram(1, 0, 0.661*2);
    gammaCDF.setBinContents(std::vector<double>{1});
    const Cylinder sourceCylinder = Cylinder(QVector3D(25, 25, 8.4478), 5.63372, 1.4097);
    const Source gammaSource = Source(sourceCylinder, gammaCDF);

    Particle prtl = gammaSource.createParticle();
    EXPECT_DOUBLE_EQ(prtl.ergE, 0.661);
    EXPECT_TRUE(sourceCylinder.contain(prtl.pos));
    EXPECT_DOUBLE_EQ(prtl.dir.length(), 1.0);
}