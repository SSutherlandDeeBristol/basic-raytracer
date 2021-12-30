#include "GeometryUtils.h"

namespace bv {
void Hit::correctNormal(const vec3d& rayDir) {
    frontFacing = glm::dot(rayDir, normal) < 0.0;
    normal = frontFacing ? normal : -normal;
}
}