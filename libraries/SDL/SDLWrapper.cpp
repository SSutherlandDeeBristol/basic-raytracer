#include "SDL.h" // Annoying but forward declaration...

#include "SDLWrapper.h"
#include "glm/vec3.hpp"

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <mutex>
#include <vector>

namespace bv {
class SDLScreen::Impl {
public:
    Impl(const int width, const int height, const std::string& title, const bool fullscreen = false) {
        SDL_version compiled;
        SDL_VERSION(&compiled);
        if (compiled.major < 2) {
            throw std::runtime_error("Could not initialise SDL: Requires SDL2 headers (current "
                                     + std::to_string((int) compiled.major) + "." + std::to_string((int) compiled.minor)
                                     + ")");
        }

        SDL_version linked;
        SDL_GetVersion(&linked);
        if (linked.major < 2) {
            throw std::runtime_error("Could not initialise SDL: Requires SDL2 runtime (current "
                                     + std::to_string((int) linked.major) + "." + std::to_string((int) linked.minor)
                                     + ")");
        }

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            throw std::runtime_error("Could not initialise SDL: " + std::string(SDL_GetError()));
        }

        this->width = width;
        this->height = height;
        buffer = new uint32_t[width * height];
        memset(buffer, 0, width * height * sizeof(uint32_t));

        uint32_t flags = SDL_WINDOW_OPENGL;
        if (fullscreen) {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        window = SDL_CreateWindow(title.c_str(),
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  width, height, flags);
        if (!window) {
            throw std::runtime_error("Could not set video mode: "
                                     + std::string(SDL_GetError()));
        }

        flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
        renderer = SDL_CreateRenderer(window, -1, flags);
        if (!renderer) {
            throw std::runtime_error("Could not create renderer: "
                                     + std::string(SDL_GetError()));
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, width, height);

        texture = SDL_CreateTexture(renderer,
                                    SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STATIC,
                                    width, height);
        if (!texture) {
            throw std::runtime_error("Could not allocate texture: "
                                     + std::string(SDL_GetError()));
        }
    }

    void putPixel(const int x, const int y, const glm::vec3& colour) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            std::cout << "apa" << std::endl;
            return;
        }

        const auto r = uint32_t(std::clamp(255.0f * colour.x, 0.f, 255.f));
        const auto g = uint32_t(std::clamp(255.0f * colour.y, 0.f, 255.f));
        const auto b = uint32_t(std::clamp(255.0f * colour.z, 0.f, 255.f));

        buffer[y * width + x] = (128 << 24) + (r << 16) + (g << 8) + b;
    }

    std::vector<SDL_Event> render() {
        {
            std::lock_guard lk(bufferMutex);
            SDL_UpdateTexture(texture, NULL, buffer, width * sizeof(uint32_t));
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        const auto time = SDL_GetTicks64();
        const auto dt = time - lastFrameTime;
        std::cout << "Render time: " << dt << " ms. FPS: " << 1000.0f / std::max(dt, 1ULL) << "\n";
        lastFrameTime = time;

        std::vector<SDL_Event> events;
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            events.push_back(event);
        }

        return events;
    }

    void saveImage(const std::string& filename) {
        uint32_t rmask, gmask, bmask, amask;

        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            amask = 0x00 << 0;
            rmask = 0xff << 8;
            gmask = 0xff << 16;
            bmask = 0xff << 24;
        } else {
            amask = 0x00 << 24;
            rmask = 0xff << 16;
            gmask = 0xff << 8;
            bmask = 0xff << 0;
        }

        SDL_Surface *surf;
        {
            std::lock_guard lk(bufferMutex);
            surf = SDL_CreateRGBSurfaceFrom((void *) buffer, width, height,
                                            32, width * sizeof(uint32_t),
                                            rmask, gmask, bmask, amask);
        }

        if (SDL_SaveBMP(surf, filename.c_str()) != 0) {
            std::cout << "Failed to save image: "
                      << SDL_GetError() << "\n";
        }
    }

    ~Impl() {
        delete[] buffer;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int height;
    int width;
    uint32_t *buffer;
    std::mutex bufferMutex;

    uint64_t lastFrameTime = 0;
};

SDLScreen::SDLScreen(const int width, const int height, const std::string& title, const bool fullscreen)
    : impl(std::make_unique<Impl>(width, height, title, fullscreen)) {}

void SDLScreen::putPixel(const int x, const int y, const glm::vec3 &colour) {
    impl->putPixel(x, y, colour);
}

std::vector<SDL_Event> SDLScreen::render() {
    return impl->render();
}

void SDLScreen::saveImage(const std::string &filename) {
    impl->saveImage(filename);
}

SDLScreen::~SDLScreen() = default;
}