#pragma once

#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace bv {

    template<typename T> using vec2 = glm::vec<2, T, glm::defaultp>;
    template<typename T> using vec3 = glm::vec<3, T, glm::defaultp>;
    template<typename T> using vec4 = glm::vec<4, T, glm::defaultp>;
    template<typename T> using mat3x3 = glm::mat<3, 3, T, glm::defaultp>;
    template<typename T> using mat3x4 = glm::mat<4, 3, T, glm::defaultp>;
    template<typename T> using mat4x4 = glm::mat<4, 4, T, glm::defaultp>;

    using vec4d = glm::vec<4, double, glm::defaultp>;
    using vec3d = glm::vec<3, double, glm::defaultp>;
    using vec2d = glm::vec<2, double, glm::defaultp>;
    using vec4f = glm::vec<4, float, glm::defaultp>;
    using vec3f = glm::vec<3, float, glm::defaultp>;
    using vec2f = glm::vec<2, float, glm::defaultp>;
    using vec2i = glm::vec<2, int, glm::defaultp>;
    using mat3x3d = glm::mat<3, 3, double, glm::defaultp>;
    using mat3d = glm::mat<3, 3, double, glm::defaultp>;
    using mat4d = glm::mat<4, 4, double, glm::defaultp>;

}
