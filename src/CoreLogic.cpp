//
// Created by Andrea Ferrario on 17/10/25.
//

#include <CoreLogic.hpp>
#include <fstream>

Vec3f CoreLogic::trace(const Vec3f& rayorig, const Vec3f& raydir, const std::vector<Sphere>& spheres, const int& depth)
{
    float tnear = INFINITY;
    const Sphere* sphere = nullptr;
    // find intersection of this ray with the sphere in the scene
    for (unsigned i = 0; i < spheres.size(); ++i)
    {
        float t0 = INFINITY, t1 = INFINITY;
        if (spheres[i].intersect(rayorig, raydir, t0, t1))
        {
            if (t0 < 0) { t0 = t1; }
            if (t0 < tnear)
            {
                tnear = t0;
                sphere = &spheres[i];
            }
        }
    }
    // if there's no intersection return black or background color
    if (!sphere)
    {
        return Vec3f(2);
    }
    auto surfaceColor = Vec3f(0);
    const Vec3f point_of_intersection = rayorig + raydir * tnear;
    Vec3f nhit = point_of_intersection - sphere->getCenter();
    nhit.normalize();

    /**
     * If the normal and the view direction are not opposite to each other
     * reverse the normal direction. That also means we are inside the sphere so set
     * the inside bool to true. Finally reverse the sign of IdotN which we want positive
     */
    constexpr float bias = 1e-4;
    bool inside = false;
    if (raydir.dot(nhit) > 0) nhit = -nhit, inside = true;
    if ((sphere->getTransparency() > 0 || sphere->getReflection() > 0) && depth < MAX_RAY_DEPTH)
    {
        const float facingratio = -raydir.dot(nhit);
        const float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
        Vec3f refldir = raydir - nhit * 2 * raydir.dot(nhit);
        refldir.normalize();
        const Vec3f reflection = trace(point_of_intersection + nhit * bias, refldir, spheres, depth + 1);
        auto refraction = Vec3f(0);
        if (sphere->getTransparency())
        {
            constexpr float ior = 1.1;
            const float eta = (inside) ? ior : 1 / ior;
            const float cosi = -nhit.dot(raydir);
            const float k = 1 - eta * eta * (1 - cosi * cosi);
            Vec3f refrdir = raydir * eta + nhit * (eta * cosi - sqrt(k));
            refrdir.normalize();
            refraction = trace(point_of_intersection - nhit * bias, refrdir, spheres, depth + 1);
        }
        surfaceColor = (
            reflection * fresneleffect +
            refraction * (1 - fresneleffect) * sphere->getTransparency()) * sphere->getSurfaceColor();
    }
    else
    {
        for (unsigned i = 0; i < spheres.size(); ++i)
        {
            if (spheres[i].getEmissionColor().getX() > 0)
            {
                // this is a light
                auto transmission = Vec3f(1);
                Vec3f lightDirection = spheres[i].getCenter() - point_of_intersection;
                lightDirection.normalize();
                for (unsigned j = 0; j < spheres.size(); ++j)
                {
                    if (i != j)
                    {
                        float t0, t1;
                        if (spheres[j].intersect(point_of_intersection + nhit * bias, lightDirection, t0, t1))
                        {
                            transmission = Vec3f(0);
                            break;
                        }
                    }
                }
                surfaceColor += sphere->getSurfaceColor() * transmission *
                    std::max(static_cast<float>(0), nhit.dot(lightDirection)) * spheres[i].getEmissionColor().getX();
            }
        }
    }

    return surfaceColor + sphere->getEmissionColor();
}


float CoreLogic::mix(const float& a, const float& b, const float& mix)
{
    return b * mix + a * (1 - mix);
}

void CoreLogic::render(const std::vector<Sphere>& spheres)
{
    constexpr unsigned width = 640;
    constexpr unsigned height = 480;
    Vec3f *image = new Vec3f[width * height], *pixel = image;
    constexpr float invWidth = 1 / static_cast<float>(width);
    constexpr float invHeight = 1 / static_cast<float>(height);
    constexpr float fov = 30;
    constexpr float aspectratio = width / static_cast<float>(height);
    const float angle = tan(M_PI * 0.5 * fov / 180.);
    // Trace rays
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x, ++pixel)
        {
            const float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            const float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3f raydir(xx, yy, -1);
            raydir.normalize();
            *pixel = trace(Vec3f(0), raydir, spheres, 0);
        }
    }
    // Save result to a PPM image
    std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i)
    {
        ofs << static_cast<unsigned char>(std::min(static_cast<float>(1), image[i].getX()) * 255) <<
            static_cast<unsigned char>(std::min(static_cast<float>(1), image[i].getY()) * 255) <<
            static_cast<unsigned char>(std::min(static_cast<float>(1), image[i].getZ()) * 255);
    }
    ofs.close();
    delete [] image;
}
