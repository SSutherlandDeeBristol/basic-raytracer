#include "Scenes.h"

#include "Geometry.h"

namespace bv {

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
std::vector<std::shared_ptr<Geometry>> createCornellBox() {
    // Defines colors:
    vec3f red(0.75f, 0.15f, 0.15f);
    vec3f yellow(0.75f, 0.75f, 0.15f);
    vec3f green(0.15f, 0.75f, 0.15f);
    vec3f cyan(0.15f, 0.75f, 0.75f);
    vec3f blue(0.15f, 0.15f, 0.75f);
    vec3f purple(0.75f, 0.15f, 0.75f);
    vec3f white(0.75f, 0.75f, 0.75f);

    std::vector<std::shared_ptr<Geometry>> geometry;
    geometry.reserve(5 * 2 * 3 + 1);

    geometry.emplace_back(createSphere(vec3d(0.4, 0.6, -0.2), 0.4, purple));

    // ---------------------------------------------------------------------------
    // Room

    float L = 555;            // Length of Cornell Box side.

    const auto createTriangleWrap = [&L](vec3d a, vec3d b, vec3d c, vec3d colour) -> std::shared_ptr<Geometry> {
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

        return createTriangle(a, b, c, colour);
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
    geometry.emplace_back(createTriangleWrap(C, B, A, white));
    geometry.emplace_back(createTriangleWrap(C, D, B, white));

    // Left wall
    geometry.emplace_back(createTriangleWrap(A, E, C, green));
    geometry.emplace_back(createTriangleWrap(C, E, G, green));

    // Right wall
    geometry.emplace_back(createTriangleWrap(F, B, D, red));
    geometry.emplace_back(createTriangleWrap(H, F, D, red));

    // Ceiling
    geometry.emplace_back(createTriangleWrap(E, F, G, white));
    geometry.emplace_back(createTriangleWrap(F, H, G, white));

    // Back wall
    geometry.emplace_back(createTriangleWrap(G, D, C, white));
    geometry.emplace_back(createTriangleWrap(G, H, D, white));

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
    // geometry.push_back( Triangle(E,B,A,white,matteWhite) );
    // geometry.push_back( Triangle(E,F,B,white,matteWhite) );
    //
    // // Front
    // geometry.push_back( Triangle(F,D,B,white,matteWhite) );
    // geometry.push_back( Triangle(F,H,D,white,matteWhite) );
    //
    // // BACK
    // geometry.push_back( Triangle(H,C,D,white,matteWhite) );
    // geometry.push_back( Triangle(H,G,C,white,matteWhite) );
    //
    // // LEFT
    // geometry.push_back( Triangle(G,E,C,white,matteWhite) );
    // geometry.push_back( Triangle(E,A,C,white,matteWhite) );
    //
    // // TOP
    // geometry.push_back( Triangle(G,F,E,white,matteWhite) );
    // geometry.push_back( Triangle(G,H,F,white,matteWhite) );

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
    // geometry.push_back( Triangle(E,B,A,cyan) );
    // geometry.push_back( Triangle(E,F,B,cyan) );

    // // Front
    // geometry.push_back( Triangle(F,D,B,cyan) );
    // geometry.push_back( Triangle(F,H,D,cyan) );

    // // BACK
    // geometry.push_back( Triangle(H,C,D,cyan) );
    // geometry.push_back( Triangle(H,G,C,cyan) );

    // // LEFT
    // geometry.push_back( Triangle(G,E,C,cyan) );
    // geometry.push_back( Triangle(E,A,C,cyan) );

    // // TOP
    // geometry.push_back( Triangle(G,F,E,cyan) );
    // geometry.push_back( Triangle(G,H,F,cyan) );

    // ----------------------------------------------
    return geometry;
}
}