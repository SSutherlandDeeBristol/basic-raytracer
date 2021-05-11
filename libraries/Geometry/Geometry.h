#pragma once

#include <iostream>

#include "glm/vec3.hpp"

#include "BVH.h"

namespace bv {
class Geometry {
public:

    Geometry(const glm::vec3& colour)
            : colour(colour) {};

    virtual ~Geometry() {};

    virtual bool intersect(const Ray& ray, vec3d& intersectionPoint) = 0;

    virtual BoundingBox<vec3d> boundingBox() = 0;

    vec3f colour;
private:
};

class Triangle : public Geometry {
public:

    Triangle(const vec3d v1, const vec3d v2, const vec3d v3, const vec3f colour)
            : Geometry(colour), v1(v1), v2(v2), v3(v3) {}

    bool intersect(const Ray& ray, vec3d& intersectionPoint) override;

    BoundingBox<vec3d> boundingBox() override;

    vec3d normal() {
        return glm::normalize(glm::cross(v2 - v1,v3 - v1));
    }

    vec3d v1, v2, v3;
private:
};

class Sphere : public Geometry {
public:

    Sphere(const vec3d centre, const double radius, const vec3f colour)
            : Geometry(colour), centre(centre), radius(radius) {}

    bool intersect(const Ray& ray, vec3d& intersectionPoint) override;

    BoundingBox<vec3d> boundingBox() override;

    vec3d normal(const vec3d& p) {
        return glm::normalize(p - centre);
    }

    vec3d centre;
    double radius;
};
}

