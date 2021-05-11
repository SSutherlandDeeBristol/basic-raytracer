#include "BVH.h"

namespace bv {
BoundingBox<vec3d> mergeBoundingBoxes(const BoundingBox<vec3d>& bb1, const BoundingBox<vec3d>& bb2) {
    vec3d min = vec3d(std::min(bb1.min().x, bb2.min().x),
                      std::min(bb1.min().y, bb2.min().y),
                      std::min(bb1.min().z, bb2.min().z));

    vec3d max = vec3d(std::max(bb1.max().x, bb2.max().x),
                      std::max(bb1.max().y, bb2.max().y),
                      std::max(bb1.max().z, bb2.max().z));

    return BoundingBox<vec3d>(min, max);
}
}