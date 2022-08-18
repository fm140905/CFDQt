#include "geometry.h"

bool Cylinder::contain(const Vector3D& point) const 
{
    if(point.z() <= baseCenter.z() || 
        point.z() >= baseCenter.z() + height)
        return false;

    if(std::pow(point.x()-baseCenter.x(), 2) + std::pow(point.y()-baseCenter.y(), 2) >= std::pow(radius, 2))
        return false;

    return true;
}

double Cylinder::intersection(const Ray& ray) const
{
    // in this case the origin must reside in the cylinder
    // project the setup onto the xy plane
    Vector2D diffXY = baseCenter.toVector2D() - ray.getOrigin().toVector2D();
    Vector2D dirXY = ray.getDirection().toVector2D();
    double dirXYLen = dirXY.length();
    dirXY = dirXY / dirXYLen;
    double cosine = Vector2D::dotProduct(diffXY, dirXY);
    return 1 / dirXYLen * (cosine + std::sqrt(radius * radius + cosine * cosine - diffXY.lengthSquared()));
}

bool Sphere::contain(const Vector3D& point) const
{
    return (point-center).length() < radius;
}
double Sphere::intersection(const Ray& ray) const
{
    Vector3D prtl2det = center - ray.getOrigin();
    double proj = Vector3D::dotProduct(prtl2det, ray.getDirection());
    if(proj <= 0)
        return 0;
    
    double d = center.distanceToLine(ray.getOrigin(), ray.getDirection());
    if(d >= radius)
        return 0;
    return 2 * std::sqrt(radius*radius - d * d);
}