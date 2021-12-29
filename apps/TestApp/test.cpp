//#pragma clang optimize off

#include <iostream>

#include "Camera.h"
#include "Geometry.h"
#include "SDL.h"
#include "SDLauxiliary.h"
#include "Scenes.h"
#include "ThreadPool.h"
#include "Latch.h"

constexpr double SCREEN_WIDTH = 640.0;
constexpr double SCREEN_HEIGHT = 480.0;
constexpr int numSlices = 6;

namespace bv {
    bool Update(Camerad& camera) {
        static auto t = SDL_GetTicks64();
        /* Compute frame time */
        auto t2 = SDL_GetTicks64();
        auto dt = t2 - t;
        t = t2;

        std::cout << "Render time: " << dt << " ms. FPS: " << 1000.0f / std::max(dt, 1ULL) << "\n";

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return false;
            } else if (e.type == SDL_KEYDOWN) {
                int key_code = e.key.keysym.sym;
                switch(key_code) {
                    case SDLK_UP:
                        camera.updatePitch(-M_PI/18);
                        break;
                    case SDLK_DOWN:
                        camera.updatePitch(M_PI/18);
                        break;
                    case SDLK_LEFT:
                        camera.updateYaw(M_PI/18);
                        break;
                    case SDLK_RIGHT:
                        camera.updateYaw(-M_PI/18);
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

    SDLScreen screen;
    screen.init(SCREEN_WIDTH, SCREEN_HEIGHT, false);

    const auto sliceHeight = (camera.imageHeight / numSlices);

    const auto trace = [&camera, &geometry, &screen, sliceHeight](int sliceIndex) {
        const auto startY = sliceHeight * sliceIndex;
        for (int y = startY; y < startY + sliceHeight; y++) {
            for (int x = 0; x < camera.imageWidth; x++) {
                double closestDist = std::numeric_limits<double>::max();
                vec3f colour(0.0f,0.0f,0.0f);

                for (const auto& g : geometry) {
                    Intersection intersection{};

                    if (g->intersect(Ray{camera.trans, camera.directionFromPixelUnnormalised({x,y})}, intersection)) {
                        const auto dir = intersection.pos - camera.trans;
                        const auto dist = glm::dot(dir, dir);

                        if (dist < closestDist) {
                            colour = intersection.colour;
                            closestDist = dist;
                        }
                    }
                }

                screen.putPixel(x, y, colour);
            }
        }
    };

    ThreadPool threadPool(4);

    while (Update(camera)) {
        Latch latch(numSlices);

        // Raytrace
        for (int i = 0; i < numSlices; ++i) {
            threadPool.addTask([i, &trace, &latch](){
                trace(i);
                latch.countDown();
            });
        }

        latch.wait();

        screen.render();
//        screen.saveImage("mainout.bmp");
    }

    return 1;
}

