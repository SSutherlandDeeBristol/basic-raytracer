#include "Geometry.h"

namespace bv {
BoundingBox<vec3d> Triangle::boundingBox() {
    vec3d min(std::min(v1.x,std::min(v2.x,v3.x)),
              std::min(v1.y,std::min(v2.y,v3.y)),
              std::min(v1.z,std::min(v2.z,v3.z)));

    vec3d max(std::max(v1.x,std::max(v2.x,v3.x)),
              std::max(v1.y,std::max(v2.y,v3.y)),
              std::max(v1.z,std::max(v2.z,v3.z)));

    return BoundingBox<vec3d>(min, max);
}

BoundingBox<vec3d> Sphere::boundingBox() {
    return BoundingBox<vec3d>(centre - radius, centre + radius);
}

bool Triangle::intersect(const Ray& ray, vec3d& intersectionPoint) {
    const vec3d e1 = v2 - v1;
    const vec3d e2 = v3 - v1;

    const vec3d h = glm::cross(ray.dir, e2);
    const double a = glm::dot(h, e1);

    if (a > -1e-12 && a < 1e-12)
        return false;

    const double f = 1.0 / a;
    const vec3d s = ray.start - v1;
    const double u = glm::dot(f, glm::dot(s, h));

    if (u < 0.0 || u > 1.0)
        return false;

    const vec3d q = glm::cross(s, e1);
    const double v = glm::dot(f, glm::dot(ray.dir, q));

    if (v < 0.0 || u + v > 1.0)
        return false;

    const double t = glm::dot(f, glm::dot(e2, q));

    if (t > 1e-12) {
        intersectionPoint = ray.start + ray.dir * t;
        return true;
    }

    return false;
}

bool Sphere::intersect(const Ray& ray, vec3d& intersectionPoint) {
    const vec3d oc = ray.start - centre;
    const double a = glm::dot(ray.dir, ray.dir);
    const double b = 2.0 * glm::dot(oc, ray.dir);
    const double c = glm::dot(oc, oc) - radius * radius;
    const double discriminant = b*b - 4.0 * a * c;

    if (discriminant < 0.0f) {
        return false;
    } else {
        const double t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t >= 0) {
            intersectionPoint = ray.start + ray.dir * t;
            return true;
        }
    }

    return false;
}
}