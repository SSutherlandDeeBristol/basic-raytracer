#include <iostream>
#include <thread>

#include "BVH.h"
#include "Camera.h"
#include "Geometry.h"
#include "SDL2/SDL.h"
#include "SDLauxiliary.h"
#include "Scenes..h"

constexpr double SCREEN_WIDTH = 640.0;
constexpr double SCREEN_HEIGHT = 480.0;
constexpr int numSlices = 8;

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
                        camera.updatePitch(M_PI/18);
                        break;
                    case SDLK_DOWN:
                        camera.updatePitch(-M_PI/18);
                        break;
                    case SDLK_LEFT:
                        camera.updateYaw(-M_PI/18);
                        break;
                    case SDLK_RIGHT:
                        camera.updateYaw(M_PI/18);
                        break;
                    case SDLK_q:
                        camera.updateRoll(-M_PI/18);
                        break;
                    case SDLK_e:
                        camera.updateRoll(M_PI/18);
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

    const auto geometry = createCornellBox();

    if (numSlices > std::thread::hardware_concurrency())
        throw std::runtime_error("Not enough threads for numSlices.");

    screen *mainscreen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT, false );

    const auto trace = [&camera, &geometry, &mainscreen](int sliceIndex) {
        const auto startY = (camera.imageHeight / numSlices) * sliceIndex;
        for (int y = startY; y < startY + (camera.imageHeight / numSlices); y++) {
            for (int x = 0; x < camera.imageWidth; x++) {
                vec3f colour(0.0,0.0,0.0);
                double closestDist = std::numeric_limits<double>::max();

                for (const auto& g : geometry) {
                    vec3d intersectionPoint;
                    auto ray = std::make_unique<Ray>(camera.trans, camera.directionFromPixel({x,y}));

                    if (g->intersect(ray, intersectionPoint)) {

                        auto dist = (intersectionPoint - camera.trans).length();

                        if (dist < closestDist) {
                            colour = g->getColour();
                            closestDist = dist;
                        }
                    }
                }
                PutPixelSDL(mainscreen, x, y, colour);
            }
        }
    };

    while (Update(camera)) {
        // Raytrace
        std::vector<std::thread> threads;
        for (int i = 0; i < numSlices; ++i) {
            threads.emplace_back([i, &trace](){ trace(i); });
        }

        for (auto& thread : threads)
            thread.join();

        SDL_Renderframe(mainscreen);
//        SDL_SaveImage(mainscreen, "mainout.bmp");
    }

    KillSDL(mainscreen);
    return 1;
}

