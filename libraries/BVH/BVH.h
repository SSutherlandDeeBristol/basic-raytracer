#pragma once

#include <vector>

#include "GlmTypes.h"

namespace bv {
    template <typename V>
    class BoundingBox {
    public:
        BoundingBox(const V min, const V max)
            : min_(min), max_(max) {}

        const V min() const { return min_; }
        const V max() const { return max_; }

    private:
        V min_, max_;
    };

    BoundingBox<vec3d> mergeBoundingBoxes(const BoundingBox<vec3d>& bb1, const BoundingBox<vec3d>& bb2) {
        vec3d min = vec3d(std::min(bb1.min().x, bb2.min().x),
                          std::min(bb1.min().y, bb2.min().y),
                          std::min(bb1.min().z, bb2.min().z));

        vec3d max = vec3d(std::max(bb1.max().x, bb2.max().x),
                          std::max(bb1.max().y, bb2.max().y),
                          std::max(bb1.max().z, bb2.max().z));

        return BoundingBox<vec3d>(min, max);
    }

    class Ray {
    public:
        Ray(const vec3d& start, const vec3d& dir)
            : start(start), dir(dir) {}

        vec3d start, dir;
    private:
    };

    template <typename Object, typename BB>
    class BVH {
    public:

        BVH(const std::vector<Object>& objects) {
            buildTree(objects);
        }

    private:

        void buildTree(const std::vector<Object>& objects) {

        }
    };
}