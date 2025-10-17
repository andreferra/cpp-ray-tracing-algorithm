//
// Created by Andrea Ferrario on 17/10/25.
//

#include <Sphere.hpp>


bool Sphere::intersect(const Vec3f& rayorig, const Vec3f& raydir, float& t0, float& t1) const
{
    const Vec3f l = center - rayorig;
    const float tca = l.dot(raydir);
    if (tca < 0) return false;
    const float d2 = l.dot(l) - tca * tca;
    if (d2 > radius2) return false;
    const float thc = sqrt(radius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    return true;
}


Vec3f Sphere::getCenter() const
{
    return center;
}

float Sphere::getTransparency() const
{
    return transparency;
}

float Sphere::getReflection() const
{
    return reflection;
}

Vec3f Sphere::getSurfaceColor() const
{
    return surfaceColor;
}

Vec3f Sphere::getEmissionColor() const
{
    return emissionColor;
}