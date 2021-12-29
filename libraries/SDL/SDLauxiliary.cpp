#include "SDLauxiliary.h"

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <mutex>

#include "SDL.h"

namespace bv {
class SDLScreen::Impl {
public:
    Impl() {}

    void init(const int width, const int height, const bool fullscreen = false) {
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
        window = SDL_CreateWindow("Raytracer",
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

        inited = true;
    }

    void putPixel(const int x, const int y, const glm::vec3& colour) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            std::cout << "apa" << std::endl;
            return;
        }

        uint32_t r = uint32_t(std::clamp(255 * colour.x, 0.f, 255.f));
        uint32_t g = uint32_t(std::clamp(255 * colour.y, 0.f, 255.f));
        uint32_t b = uint32_t(std::clamp(255 * colour.z, 0.f, 255.f));

        buffer[y * width + x] = (128 << 24) + (r << 16) + (g << 8) + b;
    }

    void render() {
        {
            std::lock_guard lk(bufferMutex);
            SDL_UpdateTexture(texture, NULL, buffer, width * sizeof(uint32_t));
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    void saveImage(const std::string& filename) {
        if (!inited)
            throw std::runtime_error("Attempting saveImage() before init().");

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
        if (inited) {
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }

private:
    bool inited = false;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int height;
    int width;
    uint32_t *buffer;
    std::mutex bufferMutex;
};

SDLScreen::SDLScreen()
    : impl(std::make_unique<Impl>()) {}

void SDLScreen::init(const int width, const int height, const bool fullscreen) {
    impl->init(width, height, fullscreen);
}

void SDLScreen::putPixel(const int x, const int y, const glm::vec3 &colour) {
    impl->putPixel(x, y, colour);
}

void SDLScreen::render() {
    impl->render();
}

void SDLScreen::saveImage(const std::string &filename) {
    impl->saveImage(filename);
}

SDLScreen::~SDLScreen() = default;
}