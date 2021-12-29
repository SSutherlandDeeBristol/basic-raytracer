#pragma once

#include "GlmTypes.h"

#pragma once

namespace bv {
    template <typename T>
    class Camera {
    public:

        Camera() {}
        ~Camera() {}

        Camera(vec3<T> trans, T roll, T pitch, T yaw,
               T focalLength, T aspectRatio, int imageWidth, int imageHeight,
               T centreX, T centreY)
               : trans(trans), roll(roll), pitch(pitch), yaw(yaw), focalLength(focalLength), aspectRatio(aspectRatio),
                 imageWidth(imageWidth), imageHeight(imageHeight), centreX(centreX), centreY(centreY) {
            updateRotationMatrix();
        }

        mat3x4<T> projMatrix() const {
            mat3x3<T> intrinsics(focalLength, 0, centreX,
                                 0, focalLength * aspectRatio, centreY,
                                 0, 0, 1);

            mat3x4<T> extrinsics(rot[0][0], rot[0][1], rot[0][2], trans[0],
                                 rot[1][0], rot[1][1], rot[1][2], trans[1],
                                 rot[2][0], rot[2][1], rot[2][2], trans[2]);

            return intrinsics * extrinsics;
        }

        vec3<T> directionFromPixelUnnormalised(vec2<T> pixel) const {
            return rot * vec3<T>(pixel.x - centreX, pixel.y - centreY, focalLength);
        }

        vec3<T> directionFromPixel(vec2<T> pixel) const {
            return glm::normalize(rot * vec3<T>(pixel.x - centreX, pixel.y - centreY, focalLength));
        }

        void updateRotationMatrix() {
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

        void updatePitch(const T& x) {
            pitch += x;
            updateRotationMatrix();
        }

        void updateYaw(const T& y) {
            yaw += y;
            updateRotationMatrix();
        }

        void updateRoll(const T& z) {
            roll += z;
            updateRotationMatrix();
        }

        mat3x3<T> getRot() const {
            updateRotationMatrix();
            return rot;
        }

        void lookAt(const vec3<T>& to) {
            vec3<T> forward = glm::normalize(to - trans);
//            vec3<T> right = glm::normalize(glm::cross(vec3<T>{0,1,0}, forward));
//            vec3<T> up = glm::normalize(glm::cross(forward, right));

            pitch = asin(-forward.y);
            yaw = atan2(forward.x, forward.z);
            roll = 0.0;

            updateRotationMatrix();
        }

        vec3<T> right() const {
            return vec3<T>(rot[0][0], rot[1][0], rot[2][0]);
        }

        vec3<T> up() const {
            return -vec3<T>(rot[0][1], rot[1][1], rot[2][1]);
        }

        vec3<T> forward() const {
            return vec3<T>(rot[0][2], rot[1][2], rot[2][2]);
        }

        bool inImage(const vec2<T>& p) const {
            return !(p.x < 0 || p.x >= imageWidth || p.y < 0 || p.y >= imageHeight);
        }

        vec2<T> project(const vec3<T>& point) const {
            vec3<T> homoPoint = projMatrix() * vec4<T>{point, 1.0};

            vec2<T> pp = {homoPoint.x/homoPoint.z, homoPoint.y/homoPoint.z};

            return pp;
        }

        vec3<T> trans;
        T roll, pitch, yaw;
        T focalLength, aspectRatio;
        int imageWidth, imageHeight;
        T centreX, centreY;
    private:
        glm::dmat3x4 rot;
    };

    typedef Camera<float> Cameraf;
    typedef Camera<double> Camerad;
}