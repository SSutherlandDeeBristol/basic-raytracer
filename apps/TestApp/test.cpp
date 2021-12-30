//#pragma clang optimize off

#include "SDL.h"

#include "Camera.h"
#include "Geometry.h"
#include "SDLWrapper.h"
#include "Scenes.h"
#include "ThreadPool.h"
#include "Latch.h"

namespace bv {
    bool processEvents(const std::vector<SDL_Event>& events, Camerad& camera) {
        for (const auto& e : events) {
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
    const int screenWidth = 640;
    const int screenHeight = 480;
    const int numSlices = 4;

    Camerad camera({0.0, 0.0, -3.0}, 0.0, 0.0, 0.0, screenHeight, 1.0, screenWidth,
                     screenHeight, screenWidth / 2.0, screenHeight / 2.0);

    const auto geometry = createCornellBox();

    SDLScreen screen(screenWidth, screenHeight, "Basic Raytracer", false);

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

    std::vector<SDL_Event> events;

    while (processEvents(events, camera)) {
        Latch latch(numSlices);

        for (int i = 0; i < numSlices; ++i) {
            threadPool.enqueue([i, &trace, &latch](){
                trace(i);
                latch.countDown();
            });
        }

        //
        // Make sure all worker threads have finished tracing before rendering to screen.
        //
        latch.wait();

        events = screen.render();
//        screen.saveImage("mainout.bmp");
    }

    return 1;
}

