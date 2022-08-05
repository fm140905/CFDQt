/**
 * @file geometry.h
 * @brief geometry calculation involved in photon tracking and CFD
 * @version 0.1
 * @date 2021-07-26
 * 
 * @author Ming Fang
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
    /**
     * @brief Construct a new Ray object
     * 
     * @param p Origin of the ray
     * @param d Direction of the ray
     */
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
    /**
     * @brief Check if a point is in this object
     * 
     * @param point point to be checked
     * @return true if point is in this object.
     * @return false else
     */
    virtual bool contain(const QVector3D& point) const = 0;
    /**
     * @brief Get the length of intersection between this object and a given ray
     * 
     * @param ray 
     * @return double 
     */
    virtual double intersection(const Ray& ray) const = 0;
};

class Cylinder : public Shape
{
private:
    QVector3D baseCenter;
    double radius;
    double height;
public:
    /**
     * @brief Construct a new Cylinder object. The axis direction is (0, 0, 1)
     * 
     * @param p center of the bottom surface
     * @param h height of the cylinder
     * @param r radius of the cylinder
     */
    Cylinder(const QVector3D& p, const double h, const double& r)
        : baseCenter(p),
          height(h),
          radius(r)
    {
    }

    QVector3D getBaseCenter() const {return baseCenter;}
    QVector3D getAxis() const {return QVector3D(0, 0, height);}
    double getRadius() const {return radius;}
    double getHeight() const {return height;}

    bool contain(const QVector3D& point) const override;

    double intersection(const Ray& ray) const override;
};


class Sphere : public Shape
{
private:
    QVector3D center;
    double radius;
public:
    /**
     * @brief Construct a new Sphere object
     * 
     * @param c Center of the sphere
     * @param r Radius of the sphere
     */
    Sphere(const QVector3D& c, const double r) 
        : center(c), radius(r) {}
    // ~Sphere();

    QVector3D getCenter() const {return center;}
    double getRadius() const {return radius;}

    void setCenter(const QVector3D& newc) {center=newc;}
    void setRadius(const double newr) {radius=newr;}
    
    bool contain(const QVector3D& point) const override;
    double intersection(const Ray& ray) const override;
};

#endif // GEOMETRY_H
