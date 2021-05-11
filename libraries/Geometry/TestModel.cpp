#include "TestModel.h"

#include "Geometry.h"

namespace bv {
void scale(std::vector<Triangle> &triangles, float L) {
    for (size_t i = 0; i < triangles.size(); ++i) {
        triangles[i].v1 *= 2 / L;
        triangles[i].v2 *= 2 / L;
        triangles[i].v3 *= 2 / L;

        triangles[i].v1 -= vec3d(1, 1, 1);
        triangles[i].v2 -= vec3d(1, 1, 1);
        triangles[i].v3 -= vec3d(1, 1, 1);

        triangles[i].v1.x *= -1;
        triangles[i].v2.x *= -1;
        triangles[i].v3.x *= -1;

        triangles[i].v1.y *= -1;
        triangles[i].v2.y *= -1;
        triangles[i].v3.y *= -1;
    }
}

void translate(std::vector<Triangle> &triangles, double dist, vec3d dir) {
    for (size_t i = 0; i < triangles.size(); ++i) {
        triangles[i].v1 += (dir * dist);
        triangles[i].v2 += (dir * dist);
        triangles[i].v3 += (dir * dist);
    }
}

void rotate(std::vector<Triangle> &triangles, mat3x3d rotation) {
    for (size_t i = 0; i < triangles.size(); ++i) {
        triangles[i].v1 = rotation * triangles[i].v1;
        triangles[i].v2 = rotation * triangles[i].v1;
        triangles[i].v3 = rotation * triangles[i].v2;
    }
}

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
void LoadTestModel(std::vector<std::unique_ptr<Geometry>> &geometry) {
    std::vector<Triangle> triangles;
    // Defines colors:
    vec3f red(0.75f, 0.15f, 0.15f);
    vec3f yellow(0.75f, 0.75f, 0.15f);
    vec3f green(0.15f, 0.75f, 0.15f);
    vec3f cyan(0.15f, 0.75f, 0.75f);
    vec3f blue(0.15f, 0.15f, 0.75f);
    vec3f purple(0.75f, 0.15f, 0.75f);
    vec3f white(0.75f, 0.75f, 0.75f);

    triangles.clear();
    triangles.reserve(5 * 2 * 3);

    geometry.push_back(std::make_unique<Sphere>(vec3d(0.4, 0.6, -0.2), 0.4, purple));

    // ---------------------------------------------------------------------------
    // Room

    float L = 555;            // Length of Cornell Box side.

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
    triangles.push_back(Triangle(C, B, A, white));
    triangles.push_back(Triangle(C, D, B, white));

    // Left wall
    triangles.push_back(Triangle(A, E, C, green));
    triangles.push_back(Triangle(C, E, G, green));

    // Right wall
    triangles.push_back(Triangle(F, B, D, red));
    triangles.push_back(Triangle(H, F, D, red));

    // Ceiling
    triangles.push_back(Triangle(E, F, G, white));
    triangles.push_back(Triangle(F, H, G, white));

    // Back wall
    triangles.push_back(Triangle(G, D, C, white));
    triangles.push_back(Triangle(G, H, D, white));

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

    // // Front
    // triangles.push_back( Triangle(E,B,A,white,matteWhite) );
    // triangles.push_back( Triangle(E,F,B,white,matteWhite) );
    //
    // // Front
    // triangles.push_back( Triangle(F,D,B,white,matteWhite) );
    // triangles.push_back( Triangle(F,H,D,white,matteWhite) );
    //
    // // BACK
    // triangles.push_back( Triangle(H,C,D,white,matteWhite) );
    // triangles.push_back( Triangle(H,G,C,white,matteWhite) );
    //
    // // LEFT
    // triangles.push_back( Triangle(G,E,C,white,matteWhite) );
    // triangles.push_back( Triangle(E,A,C,white,matteWhite) );
    //
    // // TOP
    // triangles.push_back( Triangle(G,F,E,white,matteWhite) );
    // triangles.push_back( Triangle(G,H,F,white,matteWhite) );

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

    // // Front
    // triangles.push_back( Triangle(E,B,A,cyan) );
    // triangles.push_back( Triangle(E,F,B,cyan) );

    // // Front
    // triangles.push_back( Triangle(F,D,B,cyan) );
    // triangles.push_back( Triangle(F,H,D,cyan) );

    // // BACK
    // triangles.push_back( Triangle(H,C,D,cyan) );
    // triangles.push_back( Triangle(H,G,C,cyan) );

    // // LEFT
    // triangles.push_back( Triangle(G,E,C,cyan) );
    // triangles.push_back( Triangle(E,A,C,cyan) );

    // // TOP
    // triangles.push_back( Triangle(G,F,E,cyan) );
    // triangles.push_back( Triangle(G,H,F,cyan) );

    // ----------------------------------------------
    // Scale to the volume [-1,1]^3

    scale(triangles, L);

    for (size_t i = 0; i < triangles.size(); ++i) {
        geometry.push_back(std::make_unique<Triangle>(triangles[i]));
    }
}
}