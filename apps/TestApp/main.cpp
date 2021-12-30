//#pragma clang optimize off

#include <random>

#include "glm/gtc/random.hpp"

#include "SDL.h"

#include "Camera.h"
#include "Geometry.h"
#include "SDLWrapper.h"
#include "Scenes.h"
#include "ThreadPool.h"
#include "Latch.h"
#include "GeometryUtils.h"
#include "Material.h"

namespace bv {
    bool processEvents(const std::vector<SDL_Event>& events, Camerad& camera) {
        for (const auto& e : events) {
            if (e.type == SDL_QUIT) {
                return false;
            } else if (e.type == SDL_KEYDOWN) {
                int key_code = e.key.keysym.sym;
                switch (key_code) {
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
                    default:
                        break;
                }
            }
        }
        return true;
    }

    double randomDouble() {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator{0};
        return distribution(generator);
    }

    double randomDouble(const double min, const double max) {
        return min + (max - min) * randomDouble();
    }

    vec3f rayColour(const std::unique_ptr<Scene>& scene, const Ray& ray, const int depth) {
        vec3f black(0.0f,0.0f,0.0f);

        if (depth <= 0)
            return black;

        Hit hit{};

        if (scene->intersect(ray, hit, 1e-3, 1e12)) {
            Ray scattered{};
            vec3f attenuation = black;

            if (hit.material->scatter(ray, hit, attenuation, scattered)) {
                return attenuation * rayColour(scene, scattered, depth - 1);
            }

            return black;
        }

        const auto unitRayDir = glm::normalize(ray.dir);
        const float t = 0.5f * (unitRayDir.y + 1.0f);
        return (1.0f - t) * vec3f(1.0f, 1.0f, 1.0f) + t * vec3f(0.5f, 0.7f, 1.0f);
    }
}

int main() {
    using namespace bv;
    constexpr int screenWidth = 1200;
    constexpr int screenHeight = 800;
    constexpr int numSlices = 4;
    constexpr int numSamples = 512;
    constexpr int maxBounces = 512;
    constexpr float scale = 1.0 / numSamples;

    Camerad camera({0.0, 0.0, -3.0}, 0.0, 0.0, 0.0, screenHeight, 1.0, screenWidth,
                     screenHeight, screenWidth / 2.0, screenHeight / 2.0);

    const auto scene = createCornellBox();

    SDLScreen screen(screenWidth, screenHeight, "Basic Raytracer", false);

    const auto sliceHeight = (camera.imageHeight / numSlices);

    const auto trace = [&camera, &scene, &screen, sliceHeight](int sliceIndex) {
        const auto startY = sliceHeight * sliceIndex;

        for (int y = startY; y < startY + sliceHeight; y++) {
            for (int x = 0; x < camera.imageWidth; x++) {
                vec3f colour(0.0f, 0.0f, 0.0f);

                if (numSamples > 1) {
                    for (int i = 0; i < numSamples; ++i) {
                        const auto ray = Ray{camera.trans, camera.directionFromPixelUnnormalised({x + randomDouble(), y + randomDouble()})};
                        colour += rayColour(scene, ray, maxBounces);
                    }

                    colour.x = std::sqrt(colour.x * scale);
                    colour.y = std::sqrt(colour.y * scale);
                    colour.z = std::sqrt(colour.z * scale);
                } else {
                    colour = rayColour(scene, Ray{camera.trans, camera.directionFromPixelUnnormalised({x,y})}, maxBounces);
                }

                screen.putPixel(x, y, colour);
            }
        }
    };

    ThreadPool threadPool(4);

    std::vector<SDL_Event> events;

//    while (processEvents(events, camera)) {
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
        screen.saveImage("mainout.bmp");
//    }

    return 1;
}

