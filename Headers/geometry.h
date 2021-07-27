/**
 * @file geometry.h
 * @brief geometry calculation involved in photon tracking and CFD
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QtMath>
#include <QVector2D>
#include <QVector3D>
#include <QVector>

/**
 * @brief single-end ray is represented as x_i = origin + t * direction, t > 0
 * 
 */
class Ray
{
private:
    QVector3D origin;
    QVector3D direction; // unit vector
public:
    Ray(const QVector3D& p, const QVector3D& d)
       : origin(p), 
         direction(d.normalized()) 
    {}
    Ray(): Ray(QVector3D(0,0,0), QVector3D(0,0,0)) {}

    const QVector3D getOrigin() const {return origin;}
    const QVector3D getDirection() const {return direction;}
};

class Shape
{
public:
    virtual bool contain(const QVector3D& point) const = 0;
    virtual double intersection(const Ray& ray) const = 0;
};

class Cylinder : public Shape
{
private:
    QVector3D baseCenter;
    double radius;
    double height;
public:
    Cylinder(const QVector3D& p, const double h, const double& r)
        : baseCenter(p),
          height(h),
          radius(r)
    {
    }

    const QVector3D getBaseCenter() const {return baseCenter;}
    const QVector3D getAxis() const {return QVector3D(0, 0, height);}
    const double getRadius() const {return radius;}
    const double getHeight() const {return height;}

    bool contain(const QVector3D& point) const 
    {
        if(point.z() <= baseCenter.z() || 
           point.z() >= baseCenter.z() + height)
           return false;

        if(qPow(point.x()-baseCenter.x(), 2) + qPow(point.y()-baseCenter.y(), 2) >= qPow(radius, 2))
            return false;

        return true;
    }

    double intersection(const Ray& ray) const
    {
        // in this case the origin must reside in the cylinder
        // projectthe setup onto the xy plane
        QVector2D diffXY = baseCenter.toVector2D() - ray.getOrigin().toVector2D();
        QVector2D dirXY = ray.getDirection().toVector2D();
        double dirXYLen = dirXY.length();
        dirXY = dirXY / dirXYLen;
        double cosine = QVector2D::dotProduct(diffXY, dirXY);
        return 1 / dirXYLen * (cosine + qSqrt(radius * radius + cosine * cosine - diffXY.lengthSquared()));
    }
};

#endif // GEOMETRY_H
