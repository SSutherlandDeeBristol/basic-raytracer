#include "Mat.h"

#pragma once

namespace {

    template <typename FloatType>
    class Camera {
    public:

        Camera() {}
        ~Camera() {}

        Camera(Vec3<FloatType> trans, FloatType roll, FloatType pitch, FloatType yaw,
               FloatType focalLength, FloatType aspectRatio, int imageWidth, int imageHeight,
               FloatType centreX, FloatType centreY)
               : trans(trans), roll(roll), pitch(pitch), yaw(yaw), focalLength(focalLength), aspectRatio(aspectRatio),
                 imageWidth(imageWidth), imageHeight(imageHeight), centreX(centreX), centreY(centreY) {
            updateRotationMatrix();
        }

        const Mat34<FloatType> projMatrix() const {
            Mat3<FloatType> intrinsics(focalLength, 0, centreX,
                                       0, focalLength * aspectRatio, centreY,
                                       0, 0, 1);

            Mat34<FloatType> extrinsics(rot[0][0], rot[0][1], rot[0][2], trans[0],
                                        rot[1][0], rot[1][1], rot[1][2], trans[1],
                                        rot[2][0], rot[2][1], rot[2][2], trans[2]);

            return intrinsics * extrinsics;
        }

        const Vec3<FloatType> directionFromPixel(Vec2<FloatType> pixel) const {
            return (rot * Vec3<FloatType>(pixel.x - centreX, pixel.y - centreY, focalLength)).normalise();
        }

        const void updateRotationMatrix() {
            rot[0][0] = cos(yaw) * cos(roll);
            rot[0][1] = -cos(pitch) * sin(roll) + sin(pitch) * sin(yaw) * cos(roll);
            rot[0][2] = sin(pitch) * sin(roll) + cos(pitch) * sin(yaw) * cos(roll);

            rot[1][0] = cos(yaw) * sin(roll);
            rot[1][1] = cos(pitch) * cos(roll) + sin(pitch) * sin(yaw) * sin(roll);
            rot[1][2] = -sin(pitch) * cos(roll) + cos(pitch) * sin(yaw) * sin(roll);

            rot[2][0] = -sin(yaw);
            rot[2][1] = sin(pitch) * cos(yaw);
            rot[2][2] = cos(pitch) * cos(yaw);
        }

        const void updatePitch(const FloatType& x) {
            pitch += x;
            updateRotationMatrix();
        }

        const void updateYaw(const FloatType& y) {
            yaw += y;
            updateRotationMatrix();
        }

        const void updateRoll(const FloatType& z) {
            roll += z;
            updateRotationMatrix();
        }

        const Mat3<FloatType> getRot() {
            updateRotationMatrix();
            return rot;
        }

        const void lookAt(const Vec3<FloatType>& to) {
            Vec3<FloatType> forward = (to - trans).normalise();
            Vec3<FloatType> right = (Vec3<FloatType>{0,1,0} ^ forward).normalise();
            Vec3<FloatType> up = (forward ^ right).normalise();

            pitch = asin(-forward.y);
            yaw = atan2(forward.x, forward.z);
            roll = 0.0;

            updateRotationMatrix();
        }

        const Vec3<FloatType> right() const {
            return Vec3<FloatType>(rot[0][0], rot[1][0], rot[2][0]);
        }

        const Vec3<FloatType> up() const {
            return -Vec3<FloatType>(rot[0][1], rot[1][1], rot[2][1]);
        }

        const Vec3<FloatType> forward() const {
            return Vec3<FloatType>(rot[0][2], rot[1][2], rot[2][2]);
        }

        const bool inImage(const Vec2<FloatType>& p) {
            return !(p.x < 0 || p.x >= imageWidth || p.y < 0 || p.y >= imageHeight);
        }

        const Vec2<FloatType> project(const Vec4<FloatType>& point) {
            Vec3<FloatType> homoPoint = projMatrix() * point.homoNorm();

            Vec2<FloatType> pp = {homoPoint.x/homoPoint.z, homoPoint.y/homoPoint.z};

            return pp;
        }

        Vec3<FloatType> trans;
        FloatType roll, pitch, yaw;
        FloatType focalLength, aspectRatio;
        int imageWidth, imageHeight;
        FloatType centreX, centreY;
    private:
        Mat3<FloatType> rot;
    };

    typedef Camera<float> Cameraf;
    typedef Camera<double> Camerad;
}