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
    bool frontFacing;
    vec3f colour;
    std::shared_ptr<Material> material;

    void correctNormal(const vec3d& rayDir);
};

template <typename T>
vec3<T> reflect(const vec3<T>& v, const vec3<T>& n) {
    return v - T(2.0) * glm::dot(v, n) * n;
}

template <typename T>
vec3<T> refract(const vec3<T>& v, const vec3<T>& n, const T etaOverEtaP) {
    const auto cosTheta = std::fmin(glm::dot(-v, n), T(1.0));
    const auto rPrimePerp = etaOverEtaP * (v + cosTheta * n);
    const auto rPrimeParallel = -std::sqrt(T(1.0) - glm::dot(rPrimePerp, rPrimePerp));
    return rPrimePerp + rPrimeParallel;
}

template <typename T>
T reflectance(const T cosine, const T etaOverEtaP) {
    auto r0 = (T(1.0) - etaOverEtaP) / (T(1.0) + etaOverEtaP);
    r0 = r0 * r0;
    return r0 + (T(1.0) - r0) * std::pow((T(1.0) - cosine), T(5.0));
}
}