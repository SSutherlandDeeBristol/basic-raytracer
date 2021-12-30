#pragma once

#include <memory>

#include "GlmTypes.h"

namespace bv {

struct Hit;
struct Ray;

class Geometry {
public:
    virtual bool intersect(const Ray& ray, Hit& hit, double tMin, double tMax) = 0;

    virtual ~Geometry() = 0;
};

class Material;

std::shared_ptr<Geometry> createTriangle(const vec3d& v1, const vec3d& v2, const vec3d& v3, const std::shared_ptr<Material>& material);
std::shared_ptr<Geometry> createSphere(const vec3d& centre, double radius, const std::shared_ptr<Material>& material);
}

