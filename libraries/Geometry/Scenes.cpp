#include "Scenes.h"

#include <vector>

#include "Material.h"
#include "Geometry.h"
#include "GeometryUtils.h"

namespace bv {

class Scene::Impl {
public:
    Impl() = default;

    void add(const std::shared_ptr<Geometry>& g) {
        geometry.emplace_back(g);
    }

    bool intersect(const Ray &ray, Hit &hit, const double tMin, const double tMax) {
        auto closestDist = std::numeric_limits<double>::max();
        bool intersection = false;

        for (const auto& g : geometry) {
            Hit hit_{};
            if (g->intersect(ray, hit_, tMin, tMax)) {
                if (hit_.t < closestDist) {
                    intersection = true;
                    hit = hit_;
                    closestDist = hit_.t;
                }
            }
        }

        return intersection;
    }

    ~Impl() = default;

private:
    std::vector<std::shared_ptr<Geometry>> geometry;
};

Scene::Scene() : impl(std::make_unique<Impl>()) {}

void Scene::add(const std::shared_ptr<Geometry> &geometry) {
    impl->add(geometry);
}

bool Scene::intersect(const Ray &ray, Hit &hit, const double tMin, const double tMax) {
    return impl->intersect(ray, hit, tMin, tMax);
}

Scene::~Scene() = default;

//
//void translate(std::vector<Triangle> &triangles, double dist, vec3d dir) {
//    for (size_t i = 0; i < triangles.size(); ++i) {
//        triangles[i].v1 += (dir * dist);
//        triangles[i].v2 += (dir * dist);
//        triangles[i].v3 += (dir * dist);
//    }
//}
//
//void rotate(std::vector<Triangle> &triangles, mat3x3d rotation) {
//    for (size_t i = 0; i < triangles.size(); ++i) {
//        triangles[i].v1 = rotation * triangles[i].v1;
//        triangles[i].v2 = rotation * triangles[i].v1;
//        triangles[i].v3 = rotation * triangles[i].v2;
//    }
//}

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
std::unique_ptr<Scene> createCornellBox() {
    // Defines colors:
    vec3f red(0.75f, 0.15f, 0.15f);
//    vec3f yellow(0.75f, 0.75f, 0.15f);
    vec3f green(0.15f, 0.75f, 0.15f);
//    vec3f cyan(0.15f, 0.75f, 0.75f);
//    vec3f blue(0.15f, 0.15f, 0.75f);
//    vec3f purple(0.75f, 0.15f, 0.75f);
    vec3f white(0.75f, 0.75f, 0.75f);

    auto scene = std::make_unique<Scene>();

    scene->add(createSphere(vec3d(0.4, 0.6, -0.2), 0.4, createDielectricMaterial(1.5)));

    // ---------------------------------------------------------------------------
    // Room

    float L = 555;            // Length of Cornell Box side.

    const auto createTriangleWrap = [&L](vec3d a, vec3d b, vec3d c, vec3d colour, bool mirror = false) -> std::shared_ptr<Geometry> {
        a *= 2 / L;
        b *= 2 / L;
        c *= 2 / L;

        a -= vec3d(1, 1, 1);
        b -= vec3d(1, 1, 1);
        c -= vec3d(1, 1, 1);

        a.x *= -1;
        b.x *= -1;
        c.x *= -1;

        a.y *= -1;
        b.y *= -1;
        c.y *= -1;

        return createTriangle(a, b, c, mirror ? createMetalMaterial(colour, 0.0f) : createLambertianMaterial(colour));
    };

    vec3d A(L, 0, 0);
    vec3d B(0, 0, 0);
    vec3d C(L, 0, L);
    vec3d D(0, 0, L);

    vec3d E(L, L, 0);
    vec3d F(0, L, 0);
    vec3d G(L, L, L);
    vec3d H(0, L, L);

    // ---------------------------------------------------------------------------
    // Walls

    // Floor:
    scene->add(createTriangleWrap(C, B, A, white));
    scene->add(createTriangleWrap(C, D, B, white));

    // Left wall
    scene->add(createTriangleWrap(A, E, C, green));
    scene->add(createTriangleWrap(C, E, G, green));

    // Right wall
    scene->add(createTriangleWrap(F, B, D, red));
    scene->add(createTriangleWrap(H, F, D, red));

    // Ceiling
    scene->add(createTriangleWrap(E, F, G, white));
    scene->add(createTriangleWrap(F, H, G, white));

    // Back wall
    scene->add(createTriangleWrap(G, D, C, white));
    scene->add(createTriangleWrap(G, H, D, white));

    // ---------------------------------------------------------------------------
    // Short block

    A = vec3d(290, 0, 114);
    B = vec3d(130, 0, 65);
    C = vec3d(240, 0, 272);
    D = vec3d(82, 0, 225);

    E = vec3d(290, 165, 114);
    F = vec3d(130, 165, 65);
    G = vec3d(240, 165, 272);
    H = vec3d(82, 165, 225);

//     // Front
//    scene->add(createTriangleWrap(E,B,A,white));
//    scene->add(createTriangleWrap(E,F,B,white));
//
//     // Front
//    scene->add(createTriangleWrap(F,D,B,white));
//    scene->add(createTriangleWrap(F,H,D,white));
//
//     // BACK
//    scene->add(createTriangleWrap(H,C,D,white));
//    scene->add(createTriangleWrap(H,G,C,white));
//
//     // LEFT
//    scene->add(createTriangleWrap(G,E,C,white));
//    scene->add(createTriangleWrap(E,A,C,white));
//
//     // TOP
//    scene->add(createTriangleWrap(G,F,E,white));
//    scene->add(createTriangleWrap(G,H,F,white));

    // ---------------------------------------------------------------------------
    // Tall block

    A = vec3d(423, 0, 247);
    B = vec3d(265, 0, 296);
    C = vec3d(472, 0, 406);
    D = vec3d(314, 0, 456);

    E = vec3d(423, 330, 247);
    F = vec3d(265, 330, 296);
    G = vec3d(472, 330, 406);
    H = vec3d(314, 330, 456);

     // Front
     scene->add(createTriangleWrap(E,B,A,white, true));
     scene->add(createTriangleWrap(E,F,B,white, true));

     // Front
     scene->add(createTriangleWrap(F,D,B,white));
     scene->add(createTriangleWrap(F,H,D,white));

     // BACK
     scene->add(createTriangleWrap(H,C,D,white));
     scene->add(createTriangleWrap(H,G,C,white));

     // LEFT
     scene->add(createTriangleWrap(G,E,C,white));
     scene->add(createTriangleWrap(E,A,C,white));

     // TOP
     scene->add(createTriangleWrap(G,F,E,white));
     scene->add(createTriangleWrap(G,H,F,white));

    // ----------------------------------------------
    return scene;
}
}