#include <gtest/gtest.h>
#include "data.h"
#include "material.h"

class NeutronCrossSectionTest : public ::testing::Test
{
public:
    NeutronCrossSection* H1NeutronCrossSection;
    NeutronCrossSection* O16NeutronCrossSection;
    void SetUp() override
    {
        // std::string rootdir = "/home/mingf2/projects/2021_DTRA/";
        // std::string rootdir = "/media/ming/DATA/projects/2021_DTRA/cfdneutron/";
        std::string rootdir = getRootDir();
        H1NeutronCrossSection = new NeutronCrossSection(rootdir+"DATA/H1-total-cross-section.txt",
                                                    rootdir+"DATA/H1-elastic-scattering-cross-section.txt");
        O16NeutronCrossSection = new NeutronCrossSection(rootdir+"DATA/O16-total-cross-section.txt",
                                                     rootdir+"DATA/O16-elastic-scattering-cross-section.txt",
                                                     rootdir+"DATA/O16-elastic-scattering-PDF.txt",
                                                     rootdir+"DATA/O16-elastic-scattering-CDF.txt");
    }
    void TearDown() override
    {
        delete H1NeutronCrossSection;
        delete O16NeutronCrossSection;
    }
};

TEST_F(NeutronCrossSectionTest, constructor)
{
    EXPECT_EQ(H1NeutronCrossSection->getTotalMicroscopicCrossSectionSize(), 384-6);
    EXPECT_EQ(O16NeutronCrossSection->getTotalMicroscopicCrossSectionSize(), 2489-6);

    EXPECT_EQ(H1NeutronCrossSection->getElasticMicroscopicCrossSectionSize(), 203-8);
    EXPECT_EQ(O16NeutronCrossSection->getElasticMicroscopicCrossSectionSize(), 2084-6);

    EXPECT_EQ(H1NeutronCrossSection->getDAPDFSize().first, 1);
    EXPECT_EQ(H1NeutronCrossSection->getDAPDFSize().second, 101);
    EXPECT_EQ(O16NeutronCrossSection->getDAPDFSize().first, 1607);
    EXPECT_EQ(O16NeutronCrossSection->getDAPDFSize().second, 101);
    
    EXPECT_EQ(H1NeutronCrossSection->getDAInvCDFSize().first, 1);
    EXPECT_EQ(H1NeutronCrossSection->getDAInvCDFSize().second, 101);
    EXPECT_EQ(O16NeutronCrossSection->getDAInvCDFSize().first, 1607);
    EXPECT_EQ(O16NeutronCrossSection->getDAInvCDFSize().second, 101);
}
TEST_F(NeutronCrossSectionTest, getTotalCrossSection)
{
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getTotalMicroscopicCrossSectionAt(1e-6), 1176.39);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getTotalMicroscopicCrossSectionAt(2e7), 6.432651e-01);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getTotalMicroscopicCrossSectionAt(1), 20.7451);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getTotalMicroscopicCrossSectionAt(1e3), 20.3208);

    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getTotalMicroscopicCrossSectionAt(1e-6), 54.1555);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getTotalMicroscopicCrossSectionAt(2e7), 1.76586751);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getTotalMicroscopicCrossSectionAt(1), 3.80075);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getTotalMicroscopicCrossSectionAt(1e4), 3.77436);
}

TEST_F(NeutronCrossSectionTest, getElasticCrossSection)
{
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getElasticMicroscopicCrossSectionAt(1e-6), 1159.66);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getElasticMicroscopicCrossSectionAt(2e7), 6.432359e-01);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getElasticMicroscopicCrossSectionAt(1), 20.6923);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getElasticMicroscopicCrossSectionAt(1e3), 20.4366);

    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getElasticMicroscopicCrossSectionAt(1e-6), 54.1469);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getElasticMicroscopicCrossSectionAt(2e7), 1.043929);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getElasticMicroscopicCrossSectionAt(1), 3.80376);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getElasticMicroscopicCrossSectionAt(1e5), 3.547228);
}

TEST_F(NeutronCrossSectionTest, getDAPDF)
{
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAPDFAt(1e-6, -0.99), 0.5);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAPDFAt(2e-7, -0.99), 0.5);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAPDFAt(1, -0.99), 0.5);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAPDFAt(1e3, -0.99), 0.5);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAPDFAt(1e-6, 0.990), 0.5);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAPDFAt(2e-7, 0.999), 0.5);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAPDFAt(1, 0.999), 0.5);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAPDFAt(1e3, 0.999), 0.5);

    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getDAPDFAt(1e-6, -0.999), 0.5);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getDAPDFAt(1e-6, 0.999), 0.5);
    EXPECT_NEAR(O16NeutronCrossSection->getDAPDFAt(2e7, -0.999), 
                                                    (2.48760366e-01*0.19 + 1.98043589e-01 * 0.01 ) / 0.2,
                                                    1e-6);
    EXPECT_NEAR(O16NeutronCrossSection->getDAPDFAt(2e7, 0.997), 
                                                    (7.18468717e+00 * 0.17 + 6.02206831e+00 * 0.03) / 0.2,
                                                    1e-6);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getDAPDFAt(1, -0.995), 
                                                        (5.00000393e-01 * 0.15 + 5.00000385e-01 * 0.05) / 0.2);
    EXPECT_NEAR(O16NeutronCrossSection->getDAPDFAt(1, 0.99), 
                                                    (4.99999607e-01+4.99999615e-01) / 2,
                                                    1e-6);
    EXPECT_NEAR(O16NeutronCrossSection->getDAPDFAt(1e5, -0.992), 
                                                    (5.46588880e-01*0.12 + 5.45675678e-01*0.08) / 0.2,
                                                    1e-6);
    EXPECT_NEAR(O16NeutronCrossSection->getDAPDFAt(1e5, 0.985), 
                                                    (4.52730656e-01*0.05 + 4.53684348e-01 * 0.15) / 0.2,
                                                    1e-6);
}

TEST_F(NeutronCrossSectionTest, getDAInvCDF)
{
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAInvCDFAt(1e-6, 0.005), -0.99);
    EXPECT_DOUBLE_EQ(H1NeutronCrossSection->getDAInvCDFAt(2e-7, 0.1), -0.8);
    EXPECT_NEAR(H1NeutronCrossSection->getDAInvCDFAt(1, 0.73), 0.46, 1e-6);
    EXPECT_NEAR(H1NeutronCrossSection->getDAInvCDFAt(1e3, 0.995), 0.99, 1e-6);

    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getDAInvCDFAt(1e-6, 0.005), -0.99);
    EXPECT_NEAR(O16NeutronCrossSection->getDAInvCDFAt(1e-6, 0.995), 0.99, 1e-6);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getDAInvCDFAt(2e7, 0.005), (-1-9.46027614e-01) / 2.0);
    EXPECT_NEAR(O16NeutronCrossSection->getDAInvCDFAt(2e7, 0.999), 9.98599674e-01*0.1 + 1.0*0.9, 1e-6);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getDAInvCDFAt(1, 0.005), (-1.0-9.80000016e-01) / 2.0);
    EXPECT_NEAR(O16NeutronCrossSection->getDAInvCDFAt(1, 0.999), 9.79999984e-01*0.1 + 1.0*0.9, 1e-6);
    EXPECT_DOUBLE_EQ(O16NeutronCrossSection->getDAInvCDFAt(1e5, 0.005), (-1.0-9.81690714e-01) / 2.0);
    EXPECT_NEAR(O16NeutronCrossSection->getDAInvCDFAt(1e5, 0.999), 9.77937446e-01*0.1 + 1.0*0.9, 1e-6);
}

class MaterialTest : public testing::Test
{
public:
    Material* water;
    void SetUp() override
    {
        // std::string rootdir = "/home/mingf2/projects/2021_DTRA/";
        // std::string rootdir = "/media/ming/DATA/projects/2021_DTRA/cfdneutron/";
        std::string rootdir = getRootDir();
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
        water = new Material(waterDensity, 18, {{2, H1}, {1, O16}});
    }
    void TearDown() override
    {
        delete water;
    }
};


TEST_F(MaterialTest, getPhotonCrossSection)
{
    const double waterDensity = 0.99; // g cm^-3

    EXPECT_DOUBLE_EQ(water->getDensity(), waterDensity);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getBinWidth(), 0.01);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getMaxE(), 1.5);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getMinE(), 0.1);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getNbins(), 141);

    double energy = 0.08;
    EXPECT_DOUBLE_EQ(water->getPhotonTotalAtten(energy), waterDensity*0.17052354215024373);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getComptonOverTotal(energy), 0.9523924611610344);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getTotalComptonIntegral(energy), 3.9504088710);

    energy = 0.102;
    EXPECT_DOUBLE_EQ(water->getPhotonTotalAtten(energy), waterDensity*0.17052354215024373);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getComptonOverTotal(energy), 0.9523924611610344);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getTotalComptonIntegral(energy), 3.9504088710);
    
    energy = 1.502;
    EXPECT_DOUBLE_EQ(water->getPhotonTotalAtten(energy), waterDensity*0.05743292677721608);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getComptonOverTotal(energy), 0.9978239201259899);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getTotalComptonIntegral(energy), 1.3755435660);
    
    energy = 1.499;
    EXPECT_DOUBLE_EQ(water->getPhotonTotalAtten(energy), waterDensity*0.05743292677721608);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getComptonOverTotal(energy), 0.9978239201259899);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getTotalComptonIntegral(energy), 1.3755435660);

    energy = 0.496;
    EXPECT_DOUBLE_EQ(water->getPhotonTotalAtten(energy), waterDensity*0.09674106896992332);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getComptonOverTotal(energy), 0.9974596785776115);
    EXPECT_DOUBLE_EQ(water->getPhotonCrossSection().getTotalComptonIntegral(energy), 2.3182732150);
}

TEST_F(MaterialTest, getNeutronCrossSection)
{
    const double rho_M_NA = 0.99 / 18 * 0.60221409; // 1e24 * cm^-3
    EXPECT_NEAR(water->getNeutronTotalAtten(1e3), rho_M_NA * (3.79339 + 2 * 2.033079999999999998e+01), 1e-6);
}