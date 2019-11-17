#include "BVH.h"
#include <iostream>

#pragma once

class Geometry {
public:

    Geometry(const Vec3f& colour)
        : colour(colour) {};

    virtual ~Geometry() {};

    virtual bool intersect(const Ray& ray, Vec3d& intersectionPoint) = 0;

    virtual BoundingBox<Vec3d> boundingBox() = 0;

    Vec3f colour;
private:
};

class Triangle : public Geometry {
public:

    Triangle(const Vec3d v1, const Vec3d v2, const Vec3d v3, const Vec3f colour)
        : v1(v1), v2(v2), v3(v3), Geometry(colour) {
    }

    bool intersect(const Ray& ray, Vec3d& intersectionPoint) override;

    BoundingBox<Vec3d> boundingBox() override;

    Vec3d normal() {
        return ((v2-v1) ^ (v3 - v1)).normalise();
    }

    Vec3d v1, v2, v3;
private:
};

class Sphere : public Geometry {
public:

    Sphere(const Vec3d centre, const double radius, const Vec3f colour)
        : centre(centre), radius(radius), Geometry(colour) {
    }

    bool intersect(const Ray& ray, Vec3d& intersectionPoint) override;

    BoundingBox<Vec3d> boundingBox() override;

    Vec3d normal(const Vec3d& p) {
        return (p - centre).normalise();
    }

    Vec3d centre;
    double radius;
};

BoundingBox<Vec3d> Triangle::boundingBox() {
    Vec3d min(std::min(v1.x,std::min(v2.x,v3.x)),
                std::min(v1.y,std::min(v2.y,v3.y)),
                std::min(v1.z,std::min(v2.z,v3.z)));

    Vec3d max(std::max(v1.x,std::max(v2.x,v3.x)),
                std::max(v1.y,std::max(v2.y,v3.y)),
                std::max(v1.z,std::max(v2.z,v3.z)));

    return BoundingBox<Vec3d>(min, max);
}

BoundingBox<Vec3d> Sphere::boundingBox() {
    return BoundingBox<Vec3d>(centre - radius, centre + radius);
}

bool Triangle::intersect(const Ray& ray, Vec3d& intersectionPoint) {
    const Vec3d e1 = v2 - v1;
    const Vec3d e2 = v3 - v1;

    const Vec3d h = ray.dir ^ e2;
    const double a = h * e1;

    if (a > -1e-12 && a < 1e-12)
        return false;

    const double f = 1.0/a;
    const Vec3d s = ray.start - v1;
    const double u = f * (s * h);

    if (u < 0.0 || u > 1.0)
        return false;

    const Vec3d q = s ^ e1;
    const double v = f * (ray.dir * q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    const double t = f * (e2 * q);

    if (t > 1e-12) {
        intersectionPoint = ray.start + ray.dir * t;
        return true;
    }

    return false;
}

bool Sphere::intersect(const Ray& ray, Vec3d& intersectionPoint) {
  const Vec3d oc = ray.start - centre;
  const double a = ray.dir * ray.dir;
  const double b = 2.0 * (oc * ray.dir);
  const double c = oc * oc - radius * radius;
  const double discriminant = b*b - 4.0*a*c;

  if (discriminant < 0.0f) {
    return false;
  } else {
    const float t = (-b - sqrt(discriminant)) / (2.0*a);
    if (t >= 0) {
      intersectionPoint = ray.start + ray.dir * t;
      return true;
    }
  }

  return false;
}