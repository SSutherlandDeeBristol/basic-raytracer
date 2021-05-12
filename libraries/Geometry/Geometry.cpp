#include "Geometry.h"

#include "BVH.h"

namespace bv {

class Triangle : public Geometry {
public:

    Triangle(const vec3d v1, const vec3d v2, const vec3d v3, const vec3f colour)
            : Geometry(colour), v1(v1), v2(v2), v3(v3) {
        normal = glm::normalize(glm::cross(v2 - v1,v3 - v1));
    }

    std::optional<Intersection> intersect(const std::unique_ptr<Ray>& ray, vec3d& intersectionPoint) override {
        const vec3d e1 = v2 - v1;
        const vec3d e2 = v3 - v1;

        const vec3d h = glm::cross(ray->dir, e2);
        const double a = glm::dot(h, e1);

        if (a > -1e-12 && a < 1e-12)
            return std::nullopt;

        const double f = 1.0 / a;
        const vec3d s = ray->start - v1;
        const double u = glm::dot(f, glm::dot(s, h));

        if (u < 0.0 || u > 1.0)
            return std::nullopt;

        const vec3d q = glm::cross(s, e1);
        const double v = glm::dot(f, glm::dot(ray->dir, q));

        if (v < 0.0 || u + v > 1.0)
            return std::nullopt;

        const double t = glm::dot(f, glm::dot(e2, q));

        if (t > 1e-12) {
            intersectionPoint = ray->start + ray->dir * t;
            return Intersection{intersectionPoint, normal};
        }

        return std::nullopt;
    }

    std::pair<vec3d, vec3d> getBoundingBox() override {
        vec3d min(std::min(v1.x,std::min(v2.x,v3.x)),
                  std::min(v1.y,std::min(v2.y,v3.y)),
                  std::min(v1.z,std::min(v2.z,v3.z)));

        vec3d max(std::max(v1.x,std::max(v2.x,v3.x)),
                  std::max(v1.y,std::max(v2.y,v3.y)),
                  std::max(v1.z,std::max(v2.z,v3.z)));

        return {min, max};
    }

    ~Triangle() = default;

private:
    vec3d v1, v2, v3;
    vec3d normal;
};

class Sphere : public Geometry {
public:

    Sphere(const vec3d centre, const double radius, const vec3f colour)
            : Geometry(colour), centre(centre), radius(radius) {}

    std::optional<Intersection> intersect(const std::unique_ptr<Ray>& ray, vec3d& intersectionPoint) override {
        const vec3d oc = ray->start - centre;
        const double a = glm::dot(ray->dir, ray->dir);
        const double b = 2.0 * glm::dot(oc, ray->dir);
        const double c = glm::dot(oc, oc) - radius * radius;
        const double discriminant = b*b - 4.0 * a * c;

        if (discriminant >= 0.0f) {
            const double t = (-b - sqrt(discriminant)) / (2.0 * a);
            if (t >= 0) {
                intersectionPoint = ray->start + ray->dir * t;
                return Intersection{intersectionPoint, intersectionPoint - centre};
            }
        }

        return std::nullopt;
    }

    std::pair<vec3d, vec3d> getBoundingBox() override {
        return {centre - radius, centre + radius};
    }

    ~Sphere() = default;

private:
    vec3d centre;
    double radius;
};

std::shared_ptr<Geometry> createTriangle(const vec3d v1, const vec3d v2, const vec3d v3, const vec3f colour) {
    return std::make_shared<Triangle>(v1, v2, v3, colour);
}

std::shared_ptr<Geometry> createSphere(const vec3d centre, const double radius, const vec3f colour) {
    return std::make_shared<Sphere>(centre, radius, colour);
}
}