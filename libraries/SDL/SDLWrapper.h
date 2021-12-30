#pragma once

#include <memory>

#include <glm/fwd.hpp>

namespace {
    typedef union SDL_Event SDL_Event;
}

namespace bv {
class SDLScreen {
public:
    SDLScreen(const int width, const int height, const std::string& title, const bool fullscreen = false);

    // Buffer is not protected by a mutex lock for performance reasons.
    void putPixel(const int x, const int y, const glm::vec3& colour);

    std::vector<SDL_Event> render();

    void saveImage(const std::string& filename);

    ~SDLScreen();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};
}