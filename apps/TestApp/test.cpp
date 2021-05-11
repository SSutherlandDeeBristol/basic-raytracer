#include <iostream>

#include "Camera.h"
#include "Geometry.h"
#include "SDL2/SDL.h"
#include "SDLauxiliary.h"
#include "TestModel.h"

constexpr double PI = 3.14159265359;
constexpr double SCREEN_WIDTH = 640.0;
constexpr double SCREEN_HEIGHT = 480.0;

namespace bv {
    bool Update(Camerad& camera) {
        static int t = SDL_GetTicks();
        /* Compute frame time */
        int t2 = SDL_GetTicks();
        float dt = float(t2-t);
        t = t2;

        std::cout << "Render time: " << dt << " ms." << std::endl;

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return false;
            } else if (e.type == SDL_KEYDOWN) {
                int key_code = e.key.keysym.sym;
                switch(key_code) {
                    case SDLK_UP:
                        camera.updatePitch(PI/18);
                        break;
                    case SDLK_DOWN:
                        camera.updatePitch(-PI/18);
                        break;
                    case SDLK_LEFT:
                        camera.updateYaw(-PI/18);
                        break;
                    case SDLK_RIGHT:
                        camera.updateYaw(PI/18);
                        break;
                    case SDLK_q:
                        camera.updateRoll(-PI/18);
                        break;
                    case SDLK_e:
                        camera.updateRoll(PI/18);
                        break;
                    case SDLK_r:
                        camera.lookAt({0,0,0});
                        break;
                    case SDLK_t:
                        // Reset camera position
                        camera.trans = vec3d(0,0,-3);
                        camera.lookAt({0,0,0});
                        break;
                    case SDLK_w:
                        camera.trans += camera.up() * 0.1;
                        break;
                    case SDLK_s:
                        camera.trans -= camera.up() * 0.1;
                        break;
                    case SDLK_a:
                        camera.trans -= camera.right() * 0.1;
                        break;
                    case SDLK_d:
                        camera.trans += camera.right() * 0.1;
                        break;
                    case SDLK_EQUALS:
                        camera.trans += camera.forward() * 0.1;
                        break;
                    case SDLK_MINUS:
                        camera.trans -= camera.forward() * 0.1;
                        break;
                    case SDLK_ESCAPE:
                        /* Move camera quit */
                        return false;
                }
            }
        }
        return true;
    }
}

int main() {
    using namespace bv;
    vec3d trans(0,0,-3.0);

    Camerad camera = Camerad(trans, 0.0, 0.0, 0.0, SCREEN_HEIGHT, 1.0, SCREEN_WIDTH,
                     SCREEN_HEIGHT, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

    std::vector<std::unique_ptr<Geometry>> geometry;

    LoadTestModel(geometry);

    vec3d intersectionPoint;

    screen *mainscreen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, false );

    while (Update(camera)) {
        // Raytrace
        for (int y = 0; y < camera.imageHeight; y++) {
            for (int x = 0; x < camera.imageWidth; x++) {
                vec3f colour(0.0,0.0,0.0);
                double closestDist = std::numeric_limits<double>::max();
                for (const auto& g : geometry) {
                    if (g->intersect(Ray(camera.trans, camera.directionFromPixel({x,y})), intersectionPoint)) {
                        auto dist = (intersectionPoint - camera.trans).length();
                        if (dist < closestDist) {
                            colour = g->colour;
                            closestDist = dist;
                        }
                    }
                }
                PutPixelSDL(mainscreen, x, y, colour);
            }
        }

        // Rasterise vertices
        // for (const auto& g : geometry) {
        //     if (Triangle* t = dynamic_cast<Triangle*>(g.get())) {
        //         auto pp1 = camera.project(Vec4d{(*t).v1, 1.0});
        //         auto pp2 = camera.project(Vec4d{(*t).v2, 1.0});
        //         auto pp3 = camera.project(Vec4d{(*t).v3, 1.0});

        //         if (camera.inImage(pp1))
        //             PutPixelSDL(mainscreen, pp1.x, pp1.y, 0xFFFFFF);

        //         if (camera.inImage(pp2))
        //             PutPixelSDL(mainscreen, pp2.x, pp2.y, 0xFFFFFF);

        //         if (camera.inImage(pp3))
        //             PutPixelSDL(mainscreen, pp3.x, pp3.y, 0xFFFFFF);
        //     }
        // }

        SDL_Renderframe(mainscreen);
        SDL_SaveImage(mainscreen, "mainout.bmp");
    }

    KillSDL(mainscreen);
    return 1;
}

