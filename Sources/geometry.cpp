#include "geometry.h"

bool Cylinder::contain(const QVector3D& point) const 
{
    if(point.z() <= baseCenter.z() || 
        point.z() >= baseCenter.z() + height)
        return false;

    if(qPow(point.x()-baseCenter.x(), 2) + qPow(point.y()-baseCenter.y(), 2) >= qPow(radius, 2))
        return false;

    return true;
}

double Cylinder::intersection(const Ray& ray) const
{
    // in this case the origin must reside in the cylinder
    // project the setup onto the xy plane
    QVector2D diffXY = baseCenter.toVector2D() - ray.getOrigin().toVector2D();
    QVector2D dirXY = ray.getDirection().toVector2D();
    double dirXYLen = dirXY.length();
    dirXY = dirXY / dirXYLen;
    double cosine = QVector2D::dotProduct(diffXY, dirXY);
    return 1 / dirXYLen * (cosine + qSqrt(radius * radius + cosine * cosine - diffXY.lengthSquared()));
}

bool Sphere::contain(const QVector3D& point) const
{
    return (point-center).length() < radius;
}
double Sphere::intersection(const Ray& ray) const
{
    double d = center.distanceToLine(ray.getOrigin(), ray.getDirection());
    if(d >= radius)
        return 0;
    return 2 * qSqrt(radius*radius - d * d);
}