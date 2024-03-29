#include "Material.h"

#include <algorithm>
#include <random>

#include "glm/gtc/random.hpp"
#include "glm/gtc/epsilon.hpp"

#include "GeometryUtils.h"

namespace bv {

class LambertianMaterial : public Material {
public:
    LambertianMaterial(const vec3f& colour) : colour(colour) {}

    bool scatter(const Ray&, const Hit& hit, vec3f& attenuation, Ray& scattered) const override {
        scattered.start = hit.pos;
        scattered.dir = hit.normal + glm::sphericalRand(1.0);

        if (glm::all(glm::epsilonEqual(scattered.dir, {0.0, 0.0, 0.0}, 1e-8))) {
            scattered.dir = hit.normal;
        }

        attenuation = colour;
        return true;
    }

    ~LambertianMaterial() = default;

private:
    vec3f colour;
};

class Metal : public Material {
public:
    Metal(const vec3f& albedo, const double fuzz) : albedo(albedo), fuzz(std::clamp(fuzz, 0.0, 1.0)) {}

    bool scatter(const Ray& ray, const Hit& hit, vec3f& attenuation, Ray& scattered) const override {
        const auto reflectedRay = reflect(glm::normalize(ray.dir), glm::normalize(hit.normal));

        scattered.start = hit.pos;
        scattered.dir = reflectedRay + fuzz * glm::sphericalRand(1.0);

        attenuation = albedo;

        return glm::dot(scattered.dir, hit.normal) > 0;
    }

    ~Metal() = default;

private:
    vec3f albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(const double indexOfRefraction) : indexOfRefraction(indexOfRefraction) {}

    bool scatter(const Ray& ray, const Hit& hit, vec3f& attenuation, Ray& scattered) const override {
        attenuation = vec3f{1.0f, 1.0f, 1.0f};

        const auto etaOverEtaP = hit.frontFacing ? 1.0 / indexOfRefraction : indexOfRefraction;

        const auto cosTheta = std::fmin(glm::dot(-ray.dir, hit.normal), 1.0);
        const auto sinTheta = std::sqrt(1 - cosTheta * cosTheta);

        scattered.start = hit.pos;

        if (etaOverEtaP * sinTheta > 1.0 || reflectance(cosTheta, etaOverEtaP) > dist(mt)) {
            scattered.dir = reflect(ray.dir, hit.normal);
        } else {
            scattered.dir = refract(glm::normalize(ray.dir), hit.normal, etaOverEtaP);
        }

        return true;
    }

private:
    double indexOfRefraction;
    mutable std::mt19937_64 mt{};
    mutable std::uniform_real_distribution<double> dist{0.0, 1.0};
};

std::shared_ptr<Material> createLambertianMaterial(const vec3f &colour) {
    return std::make_shared<LambertianMaterial>(colour);
}

std::shared_ptr<Material> createMetalMaterial(const vec3f &colour, const double fuzz) {
    return std::make_shared<Metal>(colour, fuzz);
}

std::shared_ptr<Material> createDielectricMaterial(const double indexOfRefraction) {
    return std::make_shared<Dielectric>(indexOfRefraction);
}

    Material::~Material() = default;
}