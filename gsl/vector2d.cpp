#include "vector2d.h"
#include "vector3d.h"
#include <cmath>

namespace gsl
{
    Vector2D::Vector2D(GLfloat x_in, GLfloat z_in) : x{x_in}, z{z_in}
    {    }

    Vector2D::Vector2D(const int v) : x{static_cast<GLfloat>(v)}, z{static_cast<GLfloat>(v)}
    {    }

    Vector2D::Vector2D(const double v) : x{static_cast<GLfloat>(v)}, z{static_cast<GLfloat>(v)}
    {    }

    Vector2D::Vector2D(const Vector2D &v): x{v.x}, z{v.z}
    {    }

    const Vector2D& Vector2D::operator=(const Vector2D &rhs)
    {
        x = rhs.getX();
        z = rhs.getZ();

        return *this;
    }

    Vector2D Vector2D::operator+(const Vector2D &rhs) const
    {
        return {x + rhs.getX(), z + rhs.getZ()};
    }

    Vector2D Vector2D::operator-(const Vector2D &rhs) const
    {
        return {x - rhs.getX(), z - rhs.getZ()};
    }

    Vector2D& Vector2D::operator+=(const Vector2D &rhs)
    {
        x += rhs.getX();
        z += rhs.getZ();

        return *this;
    }

    Vector2D& Vector2D::operator-=(const Vector2D &rhs)
    {
        x -= rhs.getX();
        z -= rhs.getZ();

        return *this;
    }

    Vector2D Vector2D::operator-() const
    {
        return {-x, -z};
    }

    Vector2D Vector2D::operator*(float lhs) const
    {
        return {x * lhs, z * lhs};
    }

    Vector2D Vector2D::operator*(Vector2D lhs) const
    {
        return {x * lhs.x, z * lhs.z};
    }

    GLfloat Vector2D::length() const
    {
        return std::sqrt(std::pow(x, 2.f) + std::pow(z, 2.f));
    }

    void Vector2D::normalize()
    {
        GLfloat l = length();

        if (l > 0.f)
        {
            x = (x / l);
            z = (z / l);
        }
    }

    Vector2D Vector2D::normalized()
    {
        Vector2D normalized;
        GLfloat l = length();

        if (l > 0.f)
        {
            normalized.setX(x / l);
            normalized.setZ(z / l);
        }

        return normalized;
    }

    GLfloat Vector2D::cross(const Vector2D &v1, const Vector2D &v2)
    {
        return std::abs((v1.getX() * v2.getZ()) - (v1.getZ() * v2.getX()));
    }

    GLfloat Vector2D::dot(const Vector2D  &v1, const Vector2D  &v2)
    {
        return ((v1.getX() * v2.getX()) + (v1.getZ() * v2.getZ()));
    }

    GLfloat Vector2D::getX() const
    {
        return x;
    }

    void Vector2D::setX(const GLfloat &value)
    {
        x = value;
    }

    GLfloat Vector2D::getZ() const
    {
        return z;
    }

    void Vector2D::setZ(const GLfloat &value)
    {
        z = value;
    }
}
