#pragma once

#include <memory>

namespace bv {
class Geometry;
struct Ray;
struct Hit;

class Scene {
public:
    Scene();

    void add(const std::shared_ptr<Geometry>& geometry);

    bool intersect(const Ray& ray, Hit& hit, double tMin, double tMax);

    ~Scene();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
std::unique_ptr<Scene> createCornellBox();
}
