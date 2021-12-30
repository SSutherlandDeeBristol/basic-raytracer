#pragma once

#include <memory>

#include "GlmTypes.h"

namespace bv {

struct Hit;
struct Ray;

class Material {
public:
    virtual bool scatter(const Ray& ray, const Hit& hit, vec3f& colour, Ray& scattered) const = 0;

    virtual ~Material() = 0;
};

std::shared_ptr<Material> createLambertianMaterial(const vec3f& colour);
std::shared_ptr<Material> createMetalMaterial(const vec3f& colour, double fuzz);
std::shared_ptr<Material> createDielectricMaterial(const double indexOfRefraction);
}
