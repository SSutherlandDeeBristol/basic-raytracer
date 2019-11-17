#ifndef TEST_MODEL_CORNEL_BOX_H
#define TEST_MODEL_CORNEL_BOX_H

// Defines a simple test model: The Cornel Box

#include "Geometry.h"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

void scale(std::vector<Triangle>& triangles, float L) {
	for( size_t i=0; i<triangles.size(); ++i ) {
		triangles[i].v1 *= 2/L;
		triangles[i].v2 *= 2/L;
		triangles[i].v3 *= 2/L;

		triangles[i].v1 -= Vec3d(1,1,1);
		triangles[i].v2 -= Vec3d(1,1,1);
		triangles[i].v3 -= Vec3d(1,1,1);

		triangles[i].v1.x *= -1;
		triangles[i].v2.x *= -1;
		triangles[i].v3.x *= -1;

		triangles[i].v1.y *= -1;
		triangles[i].v2.y *= -1;
		triangles[i].v3.y *= -1;
	}
}

void translate(std::vector<Triangle>& triangles, double dist, Vec3d dir) {
	for( size_t i=0; i<triangles.size(); ++i ) {
		triangles[i].v1 += (dir * dist);
		triangles[i].v2 += (dir * dist);
		triangles[i].v3 += (dir * dist);
	}
}

void rotate(std::vector<Triangle>& triangles, Mat3d rotation) {
	for( size_t i=0; i<triangles.size(); ++i ) {
		triangles[i].v1 =  rotation * triangles[i].v1;
		triangles[i].v2 =  rotation * triangles[i].v1;
		triangles[i].v3 =  rotation * triangles[i].v2;
	}
}

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
void LoadTestModel( std::vector<std::unique_ptr<Geometry>>& geometry) {
    std::vector<Triangle> triangles;
	// Defines colors:
	Vec3f red(    0.75f, 0.15f, 0.15f );
	Vec3f yellow( 0.75f, 0.75f, 0.15f );
	Vec3f green(  0.15f, 0.75f, 0.15f );
	Vec3f cyan(   0.15f, 0.75f, 0.75f );
	Vec3f blue(   0.15f, 0.15f, 0.75f );
	Vec3f purple( 0.75f, 0.15f, 0.75f );
	Vec3f white(  0.75f, 0.75f, 0.75f );

	triangles.clear();
	triangles.reserve( 5*2*3 );

	geometry.push_back(std::make_unique<Sphere>(Vec3d(0.4,0.6,-0.2), 0.4, purple) );

	// ---------------------------------------------------------------------------
	// Room

	float L = 555;			// Length of Cornell Box side.

	Vec3d A(L,0,0);
	Vec3d B(0,0,0);
	Vec3d C(L,0,L);
	Vec3d D(0,0,L);

	Vec3d E(L,L,0);
	Vec3d F(0,L,0);
	Vec3d G(L,L,L);
	Vec3d H(0,L,L);

  // ---------------------------------------------------------------------------
  // Walls

	// Floor:
	triangles.push_back( Triangle( C, B, A, white ) );
	triangles.push_back( Triangle( C, D, B, white ) );

	// Left wall
	triangles.push_back( Triangle( A, E, C, green ) );
	triangles.push_back( Triangle( C, E, G, green ) );

	// Right wall
	triangles.push_back( Triangle( F, B, D, red ) );
	triangles.push_back( Triangle( H, F, D, red ) );

	// Ceiling
	triangles.push_back( Triangle( E, F, G, white ) );
	triangles.push_back( Triangle( F, H, G, white ) );

	// Back wall
	triangles.push_back( Triangle( G, D, C, white ) );
	triangles.push_back( Triangle( G, H, D, white ) );

	// ---------------------------------------------------------------------------
	// Short block

	A = Vec3d(290,0,114);
	B = Vec3d(130,0, 65);
	C = Vec3d(240,0,272);
	D = Vec3d( 82,0,225);

	E = Vec3d(290,165,114);
	F = Vec3d(130,165, 65);
	G = Vec3d(240,165,272);
	H = Vec3d( 82,165,225);

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

	A = Vec3d(423,0,247);
	B = Vec3d(265,0,296);
	C = Vec3d(472,0,406);
	D = Vec3d(314,0,456);

	E = Vec3d(423,330,247);
	F = Vec3d(265,330,296);
	G = Vec3d(472,330,406);
	H = Vec3d(314,330,456);

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

  for( size_t i=0; i<triangles.size(); ++i ) {
    geometry.push_back(std::make_unique<Triangle>(triangles[i]));
  }
}

#endif
