//
// Created by Andrea Ferrario on 17/10/25.
//

#pragma once

#include "Vec3.hpp"

#define MAX_RAY_DEPTH 5

class Sphere
{

public:
    Sphere(
        const Vec3f &c,
        const float &r,
        const Vec3f &sc,
        const float &refl = 0,
        const float &transp = 0,
        const Vec3f &ec = Vec3f(0)) :
    center(c), radius(r), radius2(r*r), surfaceColor(sc), emissionColor(ec),
    transparency(transp), reflection(refl)
    {

    }

    bool intersect(const Vec3f &rayorig, const Vec3f &raydir, float &t0, float &t1) const;

    [[nodiscard]] Vec3f getCenter() const;
    [[nodiscard]] float getTransparency() const;
    [[nodiscard]] float getReflection() const;
    [[nodiscard]] Vec3f getSurfaceColor() const;
    [[nodiscard]] Vec3f getEmissionColor() const;

private:
    Vec3f center;                           /// position of the sphere
    float radius, radius2;                  /// sphere radius and radius^2
    Vec3f surfaceColor, emissionColor;      /// surface color and emission (light)
    float transparency, reflection;         /// surface transparency and reflectivity
};

