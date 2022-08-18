#include <gtest/gtest.h>
#include "geometry.h"

TEST(RayTest, constructor)
{
    Vector3D p = Vector3D(1, 1, 1);
    Vector3D d = Vector3D(1, 1, 1);
    Ray ray = Ray(p, d);
    EXPECT_EQ(ray.getOrigin(), p);
    EXPECT_NEAR(ray.getDirection().x(), d.x() / std::sqrt(3), 1e-5);
    EXPECT_NEAR(ray.getDirection().y(), d.y() / std::sqrt(3), 1e-5);
    EXPECT_NEAR(ray.getDirection().z(), d.z() / std::sqrt(3), 1e-5);
}

TEST(CylinderTest, constructor)
{
    Vector3D baseP = Vector3D(1, 1, 1);
    double h(1);
    double r(1);
    Cylinder cyl = Cylinder(baseP, h, r);
    EXPECT_EQ(cyl.getBaseCenter(), baseP);
    Vector3D axis = cyl.getAxis();
    EXPECT_DOUBLE_EQ(axis.x(), 0);
    EXPECT_DOUBLE_EQ(axis.y(), 0);
    EXPECT_DOUBLE_EQ(axis.z(), h);
    EXPECT_DOUBLE_EQ(cyl.getRadius(), r);
    EXPECT_DOUBLE_EQ(cyl.getHeight(), h);
}

TEST(CylinderTest, contain)
{
    Vector3D baseP = Vector3D(1, 1, 1);
    double h(1);
    double r(1);
    Cylinder cyl = Cylinder(baseP, h, r);

    Vector3D p1 = Vector3D(1.5, 1.5, 1.5);
    EXPECT_TRUE(cyl.contain(p1));

    Vector3D p2 = Vector3D(1.5, 2.5, 1.5);
    EXPECT_FALSE(cyl.contain(p2));

    Vector3D p3 = Vector3D(1.5, 1.5, 1);
    EXPECT_FALSE(cyl.contain(p3));
}

TEST(CylinderTest, intersection)
{
    Vector3D baseP = Vector3D(0, 0, 1);
    double h(3);
    double r(1);
    Cylinder cyl = Cylinder(baseP, h, r);

    Vector3D p = Vector3D(0, 0, 1.5);
    Vector3D d = Vector3D(3, 3, 1.5) - p;
    Ray ray = Ray(p, d);
    EXPECT_DOUBLE_EQ(cyl.intersection(ray), cyl.getRadius());

    p = Vector3D(0, 0, 2.5);
    d = Vector3D(3, 3, 1.5) - p;
    ray = Ray(p, d);
    double t = 1/std::sqrt(2) / 3;
    // EXPECT_DOUBLE_EQ(cyl.intersection(ray), t * d.length() );
    EXPECT_NEAR(cyl.intersection(ray), t * d.length(), 1e-5);
}

TEST(SphereTest, constructor)
{
    Vector3D baseP = Vector3D(1, 1, 1);
    double r(1);
    Sphere sph = Sphere(baseP, r);
    EXPECT_EQ(sph.getCenter(), baseP);
    EXPECT_DOUBLE_EQ(sph.getRadius(), r);
}

TEST(SphereTest, contain)
{
    Vector3D baseP = Vector3D(1, 1, 1);
    double r(1);
    Sphere sph = Sphere(baseP, r);

    Vector3D p1 = Vector3D(1.5, 1.5, 1.5);
    EXPECT_TRUE(sph.contain(p1));

    Vector3D p2 = Vector3D(1.5, 2.5, 1.5);
    EXPECT_FALSE(sph.contain(p2));
}

TEST(SphereTest, intersection)
{
    Vector3D baseP = Vector3D(100, 100, 10);
    double r(1);
    Sphere sph = Sphere(baseP, r);

    Vector3D p = Vector3D(25, 25, 10);
    Vector3D d = baseP - p;
    Ray ray = Ray(p, d);
    EXPECT_DOUBLE_EQ(sph.intersection(ray), 2 * sph.getRadius());

    d = Vector3D(1, 1, 1);
    ray = Ray(p, d);
    EXPECT_DOUBLE_EQ(sph.intersection(ray), 0);
}