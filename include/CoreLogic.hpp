//
// Created by Andrea Ferrario on 17/10/25.
//

#pragma once

#define MAX_RAY_DEPTH 5
#include "Sphere.hpp"

struct CoreLogic
{
    static float mix(const float &a, const float &b, const float &mix);

    static Vec3f trace(
    const Vec3f &rayorig,
    const Vec3f &raydir,
    const std::vector<Sphere> &spheres,
    const int &depth);

    static void render(const std::vector<Sphere> &spheres);
};
