#pragma once

#include <memory>

#include "GlmTypes.h"

namespace bv {
struct Ray {
    vec3d start;
    vec3d dir;
};

class Material;

struct Hit {
    double t;
    vec3d pos;
    vec3d normal;
    vec3f colour;
    std::shared_ptr<Material> material;

    void correctNormal(const vec3d& rayDir);
};

template <typename T>
vec3<T> reflect(const vec3<T>& v, const vec3<T>& n) {
    return v - 2 * glm::dot(v, n) * n;
}
}