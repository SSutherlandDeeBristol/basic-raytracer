#pragma once

#include <memory>

#include "glm/vec3.hpp"

namespace bv {

class SDLScreen {
public:
    SDLScreen();

    void init(const int width, const int height, const bool fullscreen = false);

    // Buffer is not protected by a mutex lock for performance reasons.
    void putPixel(const int x, const int y, const glm::vec3& colour);

    void render();

    void saveImage(const std::string& filename);

    ~SDLScreen();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};
}