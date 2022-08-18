#include <gtest/gtest.h>
// #include <Vector3D>
// #include <QtMath>
#include <iostream>
#include "geometry.h"

TEST(Vector3D, point2LineDist)
{
    Vector3D point(10, 10, 10);
    Vector3D origin(0, 0 ,0);
    Vector3D direction(0, 0, 1);
    EXPECT_NEAR(point.distanceToLine(origin, direction), 
                M_SQRT2 * 10, 1e-8);
}
