//
// Created by Andrea Ferrario on 17/10/25.
//


#pragma once
#include <ostream>
#include <cmath>

template<typename T>
class Vec3
{
public:
    Vec3(): x(0), y(0), z(0) {}
    explicit Vec3(T v): x(v), y(v), z(v) {}
    Vec3(T x, T y, T z): x(x), y(y), z(z) {}

    Vec3& normalize();

    Vec3 operator * (const T &f) const { return Vec3(x * f, y * f, z * f); }
    Vec3 operator * (const Vec3 &v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
    T dot(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 operator - (const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator + (const Vec3 &v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3& operator += (const Vec3 &v) { x += v.x, y += v.y, z += v.z; return *this; }
    Vec3& operator *= (const Vec3 &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
    Vec3 operator - () const { return Vec3(-x, -y, -z); }
    T length2() const { return x * x + y * y + z * z; }
    T length() const { return std::sqrt(length2()); }

    /**
     * @brief Outputs the components of the vector to a given output stream.
     *
     * This operator allows Vec3 objects to be printed directly using standard
     * output streams such as std::cout or file streams. The vector is typically
     * represented in the format (x, y, z).
     *
     * @param os The output stream where the vector will be written (e.g., std::cout).
     * @param v The Vec3 instance to be output.
     * @return A reference to the output stream (os) to allow chaining of output operations.
     */
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v)
    {
        os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
        return os;
    }

    T getX() const { return x; }
    T getY() const { return y; }
    T getZ() const { return z; }
private:
    T x, y, z;
};


typedef Vec3<float> Vec3f;


// Provide template implementation in header so it's available to all translation units.
template <typename T>
inline Vec3<T>& Vec3<T>::normalize()
{
    T nor2 = length2();
    if (nor2 > 0) {
        T invNor = T(1) / std::sqrt(nor2);
        x *= invNor; y *= invNor; z *= invNor;
    }
    return *this;
}
