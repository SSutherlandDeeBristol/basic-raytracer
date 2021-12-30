#include "Geometry.h"

#include <algorithm>

#include "GeometryUtils.h"
#include "Material.h"

namespace bv {

// https://jcgt.org/published/0005/03/03/
class PrecomputedTriangle : public Geometry {
public:
    PrecomputedTriangle(const vec3d v1, const vec3d v2, const vec3d v3, const std::shared_ptr<Material>& material)
            : e1(v2 - v1), e2(v3 - v1), material(material) {
        normal = glm::normalize(glm::cross(e1,e2));

        double x1;
        double x2;
        double num = glm::dot(v1, normal);

        if (std::fabs(normal.x) > std::fabs(normal.y) && std::fabs(normal.x) > std::fabs(normal.z)) {
            x1 = v2.y * v1.z - v2.z * v1.y;
            x2 = v3.y * v1.z - v3.z * v1.y;

            transform[0] = vec4d(0.0, e2.z / normal.x, -e2.y / normal.x, x2 / normal.x);
            transform[1] = vec4d(0.0, -e1.z / normal.x, e1.y / normal.x, -x1 / normal.x);
            transform[2] = vec4d(1.0, normal.y / normal.x, normal.z / normal.x, -num / normal.x);
        } else if (std::fabs(normal.y) > std::fabs(normal.z)) {
            x1 = v2.z * v1.x - v2.x * v1.z;
            x2 = v3.z * v1.x - v3.x * v1.z;

            // b = 1 case
            transform[0] = vec4d(-e2.z / normal.y, 0.0, e2.x / normal.y, x2 / normal.y);
            transform[1] = vec4d(e1.z / normal.y, 0.0, -e1.x / normal.y, -x1 / normal.y);
            transform[2] = vec4d(normal.x / normal.y, 1.0, normal.z / normal.y, -num / normal.y);
        } else if (std::fabs(normal.z) > 0.0) {
            x1 = v2.x * v1.y - v2.y * v1.x;
            x2 = v3.x * v1.y - v3.y * v1.x;

            // c = 1 case
            transform[0] = vec4d(e2.y / normal.z, -e2.x / normal.z, 0.0, x2 / normal.z);
            transform[1] = vec4d(-e1.y / normal.z, e1.x / normal.z, 0.0, -x1 / normal.z);
            transform[2] = vec4d(normal.x / normal.z, normal.y / normal.z, 1.0f, -num / normal.z);
        } else {
            throw std::runtime_error("Error: Building precomputed-transformation triangle");
        }
    }

    bool intersect(const Ray& ray, Hit& hit, const double, const double) override {
        // Get barycentric z components of ray origin and direction for calculation of t value
        const auto transS = glm::dot(transform[2], vec4d(ray.start, 1.0)); // transform[2][0] * ray.start.x + transform[2][1] * ray.start.y + transform[2][2] * ray.start.z + transform[2][3];
        const auto transD = glm::dot(transform[2], vec4d(ray.dir, 0.0)); // transform[2][0] * ray.dir.x + transform[2][1] * ray.dir.y + transform[2][2] * ray.dir.z;

        const auto ta = -transS / transD;

        // Reject negative t values and rays parallel to triangle
//        std::cout << ta << "\n";
        if (ta <= 1e-9 || ta >= 1e6)
            return false;

        // Get global coordinates of ray's intersection with triangle's plane.
        const auto wr = vec4d(ray.start + ta * ray.dir, 1.0);

        // Calculate "x" and "y" barycentric coordinates
        const auto xg = glm::dot(transform[0], wr); // + transform[0][1] * wr[1] + transform[0][2] * wr[2] + transform[0][3];
        const auto yg = glm::dot(transform[1], wr); // + transform[1][1] * wr[1] + transform[1][2] * wr[2] + transform[1][3];

        // final intersection test
        // TODO: figure out how to scale this properly
        if (xg >= 0.0 && yg >= 0.0 && yg + xg < 4.0) {
            hit.pos = wr;
            hit.normal = normal;
            hit.material = material;
            return true;
        }

        return false;
    }

    ~PrecomputedTriangle() = default;

private:
    vec3d e1, e2;
    vec3d normal;
    std::shared_ptr<Material> material;

    mat4x3d transform;
};

class Triangle : public Geometry {
public:

    Triangle(const vec3d v1, const vec3d v2, const vec3d v3, const std::shared_ptr<Material>& material)
            : v1(v1), e1(v2 - v1), e2(v3 - v1), material(material) {
        normal = glm::normalize(glm::cross(e1,e2));
    }

    bool intersect(const Ray& ray, Hit& hit, const double tMin, const double tMax) override {
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

        if (t >= tMin && t <= tMax) {
            hit.t = t;
            hit.pos = ray.start + ray.dir * t;
            hit.normal = normal;
            hit.material = material;
            hit.correctNormal(ray.dir);
            return true;
        }

        return false;
    }

    ~Triangle() = default;

private:
    vec3d v1, e1, e2;
    vec3d normal;
    std::shared_ptr<Material> material;
};

class Sphere : public Geometry {
public:
    Sphere(const vec3d centre, const double radius, const std::shared_ptr<Material>& material)
            : centre(centre), radius(radius), material(material) {}

    bool intersect(const Ray& ray, Hit& hit, const double tMin, const double tMax) override {
        const vec3d oc = ray.start - centre;
        const double a = glm::dot(ray.dir, ray.dir);
        const double halfB = glm::dot(oc, ray.dir);
        const double c = glm::dot(oc, oc) - radius * radius;
        const double discriminant = halfB*halfB - a * c;

        if (discriminant >= 0.0) {
            const double t = (-halfB - sqrt(discriminant)) / a;
            if (t >= tMin && t <= tMax) {
                const auto intersectionPoint = ray.start + ray.dir * t;
                hit.t = t;
                hit.pos = intersectionPoint;
                hit.normal = intersectionPoint - centre;
                hit.material = material;
                hit.correctNormal(ray.dir);
                return true;
            }
        }

        return false;
    }

    ~Sphere() = default;

private:
    vec3d centre;
    double radius;
    std::shared_ptr<Material> material;
};

Geometry::~Geometry() = default;

std::shared_ptr<Geometry> createTriangle(const vec3d& v1, const vec3d& v2, const vec3d& v3, const std::shared_ptr<Material>& material) {
    return std::make_shared<Triangle>(v1, v2, v3, material);
}

std::shared_ptr<Geometry> createSphere(const vec3d& centre, const double radius, const std::shared_ptr<Material>& material) {
    return std::make_shared<Sphere>(centre, radius, material);
}
}