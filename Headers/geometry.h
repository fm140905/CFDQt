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

#include <cmath>
// #include <QtMath>
// #include <Vector2D>
// #include <Vector3D>
// #include <QVector>

class Vector2D
{
private:
    /* data */
    double X;
    double Y;
    double norm;
    void calculateNorm() {norm = std::sqrt(X*X+Y*Y);}
public:
    Vector2D():Vector2D(0, 0) {}
    Vector2D(double x_, double y_)
        : X(x_), Y(y_)
        {calculateNorm();}
    void setX(double x) {X=x; calculateNorm();}
    void setY(double y) {Y=y; calculateNorm();}

    double x() const {return X;}
    double y() const {return Y;}
    double length() const {return norm;}
    double lengthSquared() const {return norm*norm;}
    void normalize() {X=X/norm;Y=Y/norm;norm=1;}
    Vector2D normalized() const {return Vector2D(X/norm, Y/norm);}

    Vector2D operator+(const Vector2D& r) const {
        return Vector2D(this->X + r.X, this->Y + r.Y);
    }
    Vector2D operator-(const Vector2D& r) const {
        return Vector2D(this->X - r.X, this->Y - r.Y);
    }

    double operator*(const Vector2D& r) const {
        return this->X * r.X + this->Y * r.Y;
    }
    Vector2D operator/(const double d) const {
        return Vector2D(this->X / d,  this->Y / d);
    }
    Vector2D operator-() const {
        return Vector2D(-this->X, -this->Y);
    }

    Vector2D& operator*=(double d) {X*=d; Y*=d; calculateNorm(); return *this; }
    Vector2D& operator*=(const Vector2D& vector) {X*=vector.X; Y*=vector.Y; calculateNorm(); return *this; }
    // Vector2D& operator+=(double d) {X+=d; Y+=d; calculateNorm(); return *this; }
    Vector2D& operator+=(const Vector2D& vector) {X+=vector.X; Y+=vector.Y; calculateNorm(); return *this; }
    // Vector2D& operator-=(double d) {X-=d; Y-=d; calculateNorm(); return *this; }
    Vector2D& operator-=(const Vector2D& vector) {X-=vector.X; Y-=vector.Y; calculateNorm(); return *this; }
    Vector2D& operator/=(double d) {X/=d; Y/=d; calculateNorm(); return *this; }
    Vector2D& operator/=(const Vector2D& vector) {X/=vector.X; Y/=vector.Y; calculateNorm(); return *this; }

    static double dotProduct(const Vector2D& v1, const Vector2D& v2) {return v1*v2;}
};

inline bool	operator!=(const Vector2D &v1, const Vector2D &v2) {
    return v1.x() != v2.x() || v1.y() != v2.y() ;
}
inline bool	operator==(const Vector2D &v1, const Vector2D &v2) {
    return v1.x() == v2.x() && v1.y() == v2.y() ;
}
inline Vector2D operator*(double d, const Vector2D& vector) {
    return Vector2D(vector.x() * d,  vector.y() * d);
}
inline Vector2D operator*(const Vector2D& vector, double d)
{
    return Vector2D(vector.x() * d,  vector.y() * d);
}

class Vector3D
{
private:
    /* data */
    double X;
    double Y;
    double Z;
    double norm;
    void calculateNorm() {norm = std::sqrt(X*X+Y*Y+Z*Z);}
public:
    Vector3D():Vector3D(0, 0, 0) {}
    Vector3D(double x_, double y_, double z_)
        : X(x_), Y(y_), Z(z_)
        {calculateNorm();}
    
    void setX(double x) {X=x; calculateNorm();}
    void setY(double y) {Y=y; calculateNorm();}
    void setZ(double z) {Z=z; calculateNorm();}

    double x() const {return X;}
    double y() const {return Y;}
    double z() const {return Z;}
    double length() const {return norm;}
    double lengthSquared() const {return norm*norm;}
    void normalize() {X=X/norm;Y=Y/norm;Z=Z/norm;norm=1;}
    Vector3D normalized() const {return Vector3D(X/norm, Y/norm, Z/norm);}
    double distanceToLine(const Vector3D &point, const Vector3D &direction) const
    {
        Vector3D delta = point - *this;
        return std::sqrt(delta.lengthSquared() * direction.lengthSquared() - std::pow(delta*direction, 2))/direction.length();
    }

    Vector3D operator+(const Vector3D& r) const {
        return Vector3D(this->X + r.X, this->Y + r.Y, this->Z + r.Z);
    }
    Vector3D operator-(const Vector3D& r) const {
        return Vector3D(this->X - r.X, this->Y - r.Y, this->Z - r.Z);
    }

    double operator*(const Vector3D& r) const {
        return this->X * r.X + this->Y * r.Y + this->Z * r.Z;
    }
    Vector3D operator/(const double d) const {
        return Vector3D(this->X / d,  this->Y / d, this->Z / d);
    }
    Vector3D operator-() const {
        return Vector3D(-this->X, -this->Y, -this->Z);
    }

    Vector3D& operator*=(double d) {X*=d; Y*=d; Z*=d; calculateNorm(); return *this; }
    Vector3D& operator*=(const Vector3D& vector) {X*=vector.X; Y*=vector.Y; Z*=vector.Z; calculateNorm(); return *this; }
    // Vector3D& operator+=(double d) {X+=d; Y+=d; Z+=d; calculateNorm(); return *this; }
    Vector3D& operator+=(const Vector3D& vector) {X+=vector.X; Y+=vector.Y; Z+=vector.Z; calculateNorm(); return *this; }
    // Vector3D& operator-=(double d) {X-=d; Y-=d; Z-=d; calculateNorm(); return *this; }
    Vector3D& operator-=(const Vector3D& vector) {X-=vector.X; Y-=vector.Y; Z-=vector.Z; calculateNorm(); return *this; }
    Vector3D& operator/=(double d) {X/=d; Y/=d; Z/=d; calculateNorm(); return *this; }
    Vector3D& operator/=(const Vector3D& vector) {X/=vector.X; Y/=vector.Y; Z/=vector.Z; calculateNorm(); return *this; }

    Vector2D toVector2D() const {return Vector2D(X, Y);}

    static double dotProduct(const Vector3D& v1, const Vector3D& v2) {return v1*v2;}
    // ~Vector2D();
};

inline bool	operator!=(const Vector3D &v1, const Vector3D &v2) {
    return v1.x() != v2.x() || v1.y() != v2.y() || v1.z() != v2.z();
}
inline bool	operator==(const Vector3D &v1, const Vector3D &v2) {
    return v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z();
}

inline Vector3D operator*(double d, const Vector3D& vector) {
    return Vector3D(vector.x() * d,  vector.y() * d, vector.z() * d);
}
inline Vector3D operator*(const Vector3D& vector, double d)
{
    return Vector3D(vector.x() * d,  vector.y() * d, vector.z() * d);
}


/**
 * @brief single-end ray is represented as x_i = origin + t * direction, t > 0
 * 
 */
class Ray
{
private:
    Vector3D origin;
    Vector3D direction; // unit vector
public:
    /**
     * @brief Construct a new Ray object
     * 
     * @param p Origin of the ray
     * @param d Direction of the ray
     */
    Ray(const Vector3D& p, const Vector3D& d)
       : origin(p), 
         direction(d.normalized()) 
    {}
    Ray(): Ray(Vector3D(0,0,0), Vector3D(0,0,1)) {}

    const Vector3D getOrigin() const {return origin;}
    const Vector3D getDirection() const {return direction;}
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
    virtual bool contain(const Vector3D& point) const = 0;
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
    Vector3D baseCenter;
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
    Cylinder(const Vector3D& p, const double h, const double& r)
        : baseCenter(p),
          height(h),
          radius(r)
    {
    }

    const Vector3D getBaseCenter() const {return baseCenter;}
    const Vector3D getAxis() const {return Vector3D(0, 0, height);}
    const double getRadius() const {return radius;}
    const double getHeight() const {return height;}

    bool contain(const Vector3D& point) const override;

    double intersection(const Ray& ray) const override;
};


class Sphere : public Shape
{
private:
    const Vector3D center;
    const double radius;
public:
    /**
     * @brief Construct a new Sphere object
     * 
     * @param c Center of the sphere
     * @param r Radius of the sphere
     */
    Sphere(const Vector3D& c, const double r) 
        : center(c), radius(r) {}
    // ~Sphere();

    const Vector3D getCenter() const {return center;}
    const double getRadius() const {return radius;}
    
    bool contain(const Vector3D& point) const override;
    double intersection(const Ray& ray) const override;
};

#endif // GEOMETRY_H
