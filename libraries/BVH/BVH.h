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

    BoundingBox<vec3d> mergeBoundingBoxes(const BoundingBox<vec3d>& bb1, const BoundingBox<vec3d>& bb2);

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