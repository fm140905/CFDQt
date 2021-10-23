#include <gtest/gtest.h>
#include "data.h"
#include "material.h"

TEST(MaterialTest, constructor)
{
    std::string rootdir = "/home/mingf2/projects/2021_DTRA/";
    // load cross-section tables
    const PhotonCrossSection photonCrossSection(rootdir+"DATA/H2O.csv");
    const NeutronCrossSection H1NeutronCrossSection(rootdir+"DATA/H1-total-cross-section.txt",
                                                    rootdir+"DATA/H1-elatic-scattering-cross-section.txt");
    const NeutronCrossSection O16NeutronCrossSection(rootdir+"DATA/O16-total-cross-section.txt",
                                                     rootdir+"DATA/O16-elatic-scattering-cross-section.txt",
                                                     rootdir+"DATA/O16-elastic-scattering-PDF.txt",
                                                     rootdir+"DATA/O16-elastic-scattering-CDF.txt");
    // create nuclides
    const Nuclide H1(1, 1, H1NeutronCrossSection, photonCrossSection);
    const Nuclide O16(8, 16, O16NeutronCrossSection, photonCrossSection);
    // initialize material
    const double waterDensity = 0.99; // g cm^-3
    const Material water = Material(waterDensity, 18, {{2, H1}, {1, O16}});

    EXPECT_DOUBLE_EQ(water.getDensity(), waterDensity);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getBinWidth(), 0.01);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getMaxE(), 1.5);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getMinE(), 0.1);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getNbins(), 141);

    double energy = 0.08;
    EXPECT_DOUBLE_EQ(water.getPhotonTotalAtten(energy), waterDensity*0.17052354215024373);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getComptonOverTotal(energy), 0.9523924611610344);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getTotalComptonIntegral(energy), 3.9504088710);

    energy = 0.102;
    EXPECT_DOUBLE_EQ(water.getPhotonTotalAtten(energy), waterDensity*0.17052354215024373);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getComptonOverTotal(energy), 0.9523924611610344);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getTotalComptonIntegral(energy), 3.9504088710);
    
    energy = 1.502;
    EXPECT_DOUBLE_EQ(water.getPhotonTotalAtten(energy), waterDensity*0.05743292677721608);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getComptonOverTotal(energy), 0.9978239201259899);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getTotalComptonIntegral(energy), 1.3755435660);
    
    energy = 1.499;
    EXPECT_DOUBLE_EQ(water.getPhotonTotalAtten(energy), waterDensity*0.05743292677721608);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getComptonOverTotal(energy), 0.9978239201259899);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getTotalComptonIntegral(energy), 1.3755435660);

    energy = 0.496;
    EXPECT_DOUBLE_EQ(water.getPhotonTotalAtten(energy), waterDensity*0.09674106896992332);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getComptonOverTotal(energy), 0.9974596785776115);
    EXPECT_DOUBLE_EQ(water.getPhotonCrossSection().getTotalComptonIntegral(energy), 2.3182732150);
}