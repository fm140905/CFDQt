#include <gtest/gtest.h>
#include "data.h"

TEST(MaterialTest, constructor)
{
    // initialize material
    const double waterDensity = 0.99; // g cm^-3
    const Material water = Material("/media/ming/DATA/projects/2021_DTRA/waterCylinder/DATA/H2O.csv", waterDensity, 18);

    EXPECT_DOUBLE_EQ(water.getDensity(), waterDensity);
    EXPECT_DOUBLE_EQ(water.getBinWidth(), 0.01);
    EXPECT_DOUBLE_EQ(water.getMaxE(), 1.5);
    EXPECT_DOUBLE_EQ(water.getMinE(), 0.1);
    EXPECT_DOUBLE_EQ(water.getNbins(), 141);

    double energy = 0.08;
    EXPECT_DOUBLE_EQ(water.getAtten(energy), waterDensity*0.17052354215024373);
    EXPECT_DOUBLE_EQ(water.getComptonOverTotal(energy), 0.9523924611610344);
    EXPECT_DOUBLE_EQ(water.getTotalComptonIntegral(energy), 3.9504088710);

    energy = 0.102;
    EXPECT_DOUBLE_EQ(water.getAtten(energy), waterDensity*0.17052354215024373);
    EXPECT_DOUBLE_EQ(water.getComptonOverTotal(energy), 0.9523924611610344);
    EXPECT_DOUBLE_EQ(water.getTotalComptonIntegral(energy), 3.9504088710);
    
    energy = 1.502;
    EXPECT_DOUBLE_EQ(water.getAtten(energy), waterDensity*0.05743292677721608);
    EXPECT_DOUBLE_EQ(water.getComptonOverTotal(energy), 0.9978239201259899);
    EXPECT_DOUBLE_EQ(water.getTotalComptonIntegral(energy), 1.3755435660);
    
    energy = 1.499;
    EXPECT_DOUBLE_EQ(water.getAtten(energy), waterDensity*0.05743292677721608);
    EXPECT_DOUBLE_EQ(water.getComptonOverTotal(energy), 0.9978239201259899);
    EXPECT_DOUBLE_EQ(water.getTotalComptonIntegral(energy), 1.3755435660);

    energy = 0.496;
    EXPECT_DOUBLE_EQ(water.getAtten(energy), waterDensity*0.09674106896992332);
    EXPECT_DOUBLE_EQ(water.getComptonOverTotal(energy), 0.9974596785776115);
    EXPECT_DOUBLE_EQ(water.getTotalComptonIntegral(energy), 2.3182732150);
}