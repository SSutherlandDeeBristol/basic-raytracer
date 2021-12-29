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
    template<typename T> using mat4x3 = glm::mat<3, 4, T, glm::defaultp>;
    template<typename T> using mat4x4 = glm::mat<4, 4, T, glm::defaultp>;

    using vec4d = vec4<double>;
    using vec3d = vec3<double>;
    using vec2d = vec2<double>;
    using vec4f = vec4<float>;
    using vec3f = vec3<float>;
    using vec2f = vec2<float>;
    using vec2i = vec2<int>;
    using mat3x3d = mat3x3<double>;
    using mat3x4d = mat3x4<double>;
    using mat4x3d = mat4x3<double>;
    using mat3d = mat3x3<double>;
    using mat4d = mat4x4<double>;

}
