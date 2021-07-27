#include <gtest/gtest.h>
#include "geometry.h"

TEST(RayTest, constructor)
{
    QVector3D p = QVector3D(1, 1, 1);
    QVector3D d = QVector3D(1, 1, 1);
    Ray ray = Ray(p, d);
    EXPECT_EQ(ray.getOrigin(), p);
    EXPECT_NEAR(ray.getDirection().x(), d.x() / qSqrt(3), 1e-5);
    EXPECT_NEAR(ray.getDirection().y(), d.y() / qSqrt(3), 1e-5);
    EXPECT_NEAR(ray.getDirection().z(), d.z() / qSqrt(3), 1e-5);
}

TEST(CylinderTest, constructor)
{
    QVector3D baseP = QVector3D(1, 1, 1);
    double h(1);
    double r(1);
    Cylinder cyl = Cylinder(baseP, h, r);
    EXPECT_EQ(cyl.getBaseCenter(), baseP);
    QVector3D axis = cyl.getAxis();
    EXPECT_DOUBLE_EQ(axis.x(), 0);
    EXPECT_DOUBLE_EQ(axis.y(), 0);
    EXPECT_DOUBLE_EQ(axis.z(), h);
    EXPECT_DOUBLE_EQ(cyl.getRadius(), r);
    EXPECT_DOUBLE_EQ(cyl.getHeight(), h);
}

TEST(CylinderTest, contain)
{
    QVector3D baseP = QVector3D(1, 1, 1);
    double h(1);
    double r(1);
    Cylinder cyl = Cylinder(baseP, h, r);

    QVector3D p1 = QVector3D(1.5, 1.5, 1.5);
    EXPECT_TRUE(cyl.contain(p1));

    QVector3D p2 = QVector3D(1.5, 2.5, 1.5);
    EXPECT_FALSE(cyl.contain(p2));

    QVector3D p3 = QVector3D(1.5, 1.5, 1);
    EXPECT_FALSE(cyl.contain(p3));
}

TEST(CylinderTest, intersection)
{
    QVector3D baseP = QVector3D(0, 0, 1);
    double h(3);
    double r(1);
    Cylinder cyl = Cylinder(baseP, h, r);

    QVector3D p = QVector3D(0, 0, 1.5);
    QVector3D d = QVector3D(3, 3, 1.5) - p;
    Ray ray = Ray(p, d);
    EXPECT_DOUBLE_EQ(cyl.intersection(ray), cyl.getRadius());

    p = QVector3D(0, 0, 2.5);
    d = QVector3D(3, 3, 1.5) - p;
    ray = Ray(p, d);
    double t = 1/qSqrt(2) / 3;
    // EXPECT_DOUBLE_EQ(cyl.intersection(ray), t * d.length() );
    EXPECT_NEAR(cyl.intersection(ray), t * d.length(), 1e-5);
}

TEST(SphereTest, constructor)
{
    QVector3D baseP = QVector3D(1, 1, 1);
    double r(1);
    Sphere sph = Sphere(baseP, r);
    EXPECT_EQ(sph.getCenter(), baseP);
    EXPECT_DOUBLE_EQ(sph.getRadius(), r);
}

TEST(SphereTest, contain)
{
    QVector3D baseP = QVector3D(1, 1, 1);
    double r(1);
    Sphere sph = Sphere(baseP, r);

    QVector3D p1 = QVector3D(1.5, 1.5, 1.5);
    EXPECT_TRUE(sph.contain(p1));

    QVector3D p2 = QVector3D(1.5, 2.5, 1.5);
    EXPECT_FALSE(sph.contain(p2));
}

TEST(SphereTest, intersection)
{
    QVector3D baseP = QVector3D(100, 100, 10);
    double r(1);
    Sphere sph = Sphere(baseP, r);

    QVector3D p = QVector3D(25, 25, 10);
    QVector3D d = baseP - p;
    Ray ray = Ray(p, d);
    EXPECT_DOUBLE_EQ(sph.intersection(ray), 2 * sph.getRadius());

    d = QVector3D(1, 1, 1);
    ray = Ray(p, d);
    EXPECT_DOUBLE_EQ(sph.intersection(ray), 0);
}