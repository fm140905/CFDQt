#include <gtest/gtest.h>
#include <QVector3D>
#include <QtMath>
#include <iostream>

TEST(Vector3D, point2LineDist)
{
    QVector3D point(10, 10, 10);
    QVector3D origin(0, 0 ,0);
    QVector3D direction(0, 0, 1);
    EXPECT_NEAR(point.distanceToLine(origin, direction), 
                M_SQRT2 * 10, 1e-8);
}
