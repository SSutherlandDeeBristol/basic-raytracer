#pragma once

#include <memory>
#include <optional>

#include "GlmTypes.h"

namespace bv {

struct Intersection {
    vec3d pos;
    vec3d normal;
    vec3f colour;
};

struct Ray {
    vec3d start;
    vec3d dir;
};

class Geometry {
public:
    virtual bool intersect(const Ray& ray, Intersection& intersectionPoint) = 0;

    virtual ~Geometry() = 0;
};

std::shared_ptr<Geometry> createTriangle(const vec3d v1, const vec3d v2, const vec3d v3, const vec3f colour);
std::shared_ptr<Geometry> createSphere(const vec3d centre, const double radius, const vec3f colour);
}

