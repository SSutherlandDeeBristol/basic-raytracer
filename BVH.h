#include "Mat.h"

namespace {
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

    BoundingBox<Vec3d> mergeBoundingBoxes(const BoundingBox<Vec3d>& bb1, const BoundingBox<Vec3d>& bb2) {
        Vec3d min = Vec3d(std::min(bb1.min().x, bb2.min().x),
                          std::min(bb1.min().y, bb2.min().y),
                          std::min(bb1.min().z, bb2.min().z));

        Vec3d max = Vec3d(std::max(bb1.max().x, bb2.max().x),
                          std::max(bb1.max().y, bb2.max().y),
                          std::max(bb1.max().z, bb2.max().z));

        return BoundingBox<Vec3d>(min, max);
    }

    class Ray {
    public:
        Ray(const Vec3d& start, const Vec3d& dir)
            : start(start), dir(dir) {}

        Vec3d start, dir;
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