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
    Ray(): Ray(QVector3D(0,0,0), QVector3D(0,0,1)) {}

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

    bool contain(const QVector3D& point) const override;

    double intersection(const Ray& ray) const override;
};


class Sphere : public Shape
{
private:
    const QVector3D center;
    const double radius;
public:
    Sphere(const QVector3D& c, const double r) 
        : center(c), radius(r) {}
    // ~Sphere();

    const QVector3D getCenter() const {return center;}
    const double getRadius() const {return radius;}
    
    bool contain(const QVector3D& point) const override;
    double intersection(const Ray& ray) const override;
};

#endif // GEOMETRY_H
