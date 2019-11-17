#include <stdio.h>
#include <vector>

#include "Vec.h"

#pragma once

/* Row Major Matrix. */

namespace {

    template <typename FloatType, int rows, int cols>
    class Mat {
    public:

        Mat() {}

        Mat(FloatType* p) {
            for(size_t y = 0; y < rows; y++)
                for (size_t x = 0; x < cols; x++)
                    d[y][x] = p[y*cols + x];
        }

        const void create(const FloatType& p) {
            for(size_t y = 0; y < rows; y++)
                for (size_t x = 0; x < cols; x++)
                    d[y][x] = p;
        }

        const FloatType* data() const {
            return &d[0][0];
        }

        FloatType* data() {
            return &d[0][0];
        }

        const FloatType& at(const int y, const int x) const {
            if (y >= rows || y < 0 || x >= cols || x < 0)
                throw std::runtime_error("Matrix index out of bounds.");

            return d[y][x];
        }

        FloatType& at(const int y, const int x) {
            if (y >= rows || y < 0 || x >= cols || x < 0)
                throw std::runtime_error("Matrix index out of bounds.");

            return d[y][x];
        }

        Mat<FloatType, cols, rows> transpose() {
            Mat<FloatType, cols, rows> result;
            for (size_t x = 0; x < cols; x++)
                for (size_t y = 0; y < rows; y++)
                    result[x][y] = d[y][x];

            return result;
        }

        void operator*=(const FloatType& b) { for (size_t i = 0; i < rows*cols; i++) data()[i] *= b; }
        void operator/=(const FloatType& b) { for (size_t i = 0; i < rows*cols; i++) data()[i] /= b; }

        const Mat<FloatType, rows, cols> operator*(const FloatType& b) const {
            Mat<FloatType, rows, cols> r;
            for (size_t i = 0; i < rows*cols; i++) r.data()[i] = data()[i] * b;
            return r;
        }

        const Mat<FloatType, rows, cols> operator/(const FloatType& b) const {
            Mat<FloatType, rows, cols> r;
            for (size_t i = 0; i < rows*cols; i++) r.data()[i] = data()[i] / b;
            return r;
        }

        FloatType* operator[](const int& y) {
            return d[y];
        }

        const FloatType* operator[](const int& y) const {
            return d[y];
        }

        friend std::ostream& operator<<(std::ostream& os, Mat& m) {
            os << '[';
            for (size_t y = 0; y < rows; y++) {
                os << ((y > 0) ? " [" : "[");
                for (size_t x = 0; x < cols; x++) {
                    os << m[y][x] << ((x < cols - 1) ? "," : "");
                }
                os << ']';
                os << ((y < rows - 1) ? ",\n" : "");
            }
            os << "]\n";
            return os;
        }

    protected:
        FloatType d[rows][cols];
    };

    template <typename FloatType>
    class Mat34 : public Mat<FloatType, 3, 4> {
    public:
        Mat34() {
            for (size_t y = 0; y < 3; y++)
                for (size_t x = 0; x < 4; x++)
                    this->d[y][x] = 0.0;
        }

        Mat34(FloatType* p) {
            for(size_t y = 0; y < 3; y++)
                for (size_t x = 0; x < 4; x++)
                    this->d[y][x] = p[y*4 + x];
        }

        Mat34(FloatType m00, FloatType m01, FloatType m02, FloatType m03,
             FloatType m10, FloatType m11, FloatType m12, FloatType m13,
             FloatType m20, FloatType m21, FloatType m22, FloatType m23) {

            this->d[0][0] = m00; this->d[0][1] = m01; this->d[0][2] = m02; this->d[0][3] = m03;
            this->d[1][0] = m10; this->d[1][1] = m11; this->d[1][2] = m12; this->d[1][3] = m13;
            this->d[2][0] = m20; this->d[2][1] = m21; this->d[2][2] = m22; this->d[2][3] = m23;
        }

        const Vec3<FloatType> operator*(const Vec4<FloatType>& b) const {
            return Vec3<FloatType>(this->d[0][0] * b.x + this->d[0][1] * b.y + this->d[0][2] * b.z + this->d[0][3] * b.w,
                                   this->d[1][0] * b.x + this->d[1][1] * b.y + this->d[1][2] * b.z + this->d[1][3] * b.w,
                                   this->d[2][0] * b.x + this->d[2][1] * b.y + this->d[2][2] * b.z + this->d[2][3] * b.w);
        }
    private:
    };

    typedef Mat34<float> Mat34f;
    typedef Mat34<double> Mat34d;

    template <typename FloatType>
    class Mat3 : public Mat<FloatType, 3, 3> {
    public:

        Mat3() {
            for (size_t x = 0; x < 3; x++)
                for (size_t y = 0; y < 3; y++)
                    this->d[y][x] = 0.0;
        }

        Mat3(FloatType* p) {
            for(size_t y = 0; y < 3; y++)
                for (size_t x = 0; x < 3; x++)
                    this->d[y][x] = p[y*3 + x];
        }

        Mat3(FloatType m00, FloatType m01, FloatType m02,
             FloatType m10, FloatType m11, FloatType m12,
             FloatType m20, FloatType m21, FloatType m22) {

            this->d[0][0] = m00; this->d[0][1] = m01; this->d[0][2] = m02;
            this->d[1][0] = m10; this->d[1][1] = m11; this->d[1][2] = m12;
            this->d[2][0] = m20; this->d[2][1] = m21; this->d[2][2] = m22;
        }

        const FloatType determinant() const {
            FloatType deter = 0.0;

            deter += this->d[0][0] * ((this->d[1][1] * this->d[2][2])
                                    - (this->d[1][2] * this->d[2][1]));

            deter -= this->d[0][1] * ((this->d[1][0] * this->d[2][2])
                                    - (this->d[2][0] * this->d[1][2]));

            deter += this->d[0][2] * ((this->d[1][0] * this->d[2][1])
                                    - (this->d[2][0] * this->d[1][1]));

            return deter;
        }

        const Mat3<FloatType> operator*(const FloatType& b) const {
            Mat3<FloatType> r;
            for (size_t i = 0; i < 9; i++) r.data()[i] = this->data()[i] * b;
            return r;
        }

        const Mat3<FloatType> operator/(const FloatType& b) const {
            Mat3<FloatType> r;
            for (size_t i = 0; i < 9; i++) r.data()[i] = this->data()[i] / b;
            return r;
        }

        const Vec3<FloatType> operator*(const Vec3<FloatType>& b) const {
            return Vec3<FloatType>(this->d[0][0] * b.x + this->d[0][1] * b.y + this->d[0][2] * b.z,
                                    this->d[1][0] * b.x + this->d[1][1] * b.y + this->d[1][2] * b.z,
                                    this->d[2][0] * b.x + this->d[2][1] * b.y + this->d[2][2] * b.z);
        }

        const Mat3<FloatType> operator*(const Mat3<FloatType>& b) const {
            Mat3<FloatType> r;
            r[0][0] = this->d[0][0] * b[0][0] + this->d[0][1] * b[1][0] + this->d[0][2] * b[2][0];
            r[1][0] = this->d[1][0] * b[0][0] + this->d[1][1] * b[1][0] + this->d[1][2] * b[2][0];
            r[2][0] = this->d[2][0] * b[0][0] + this->d[2][1] * b[1][0] + this->d[2][2] * b[2][0];
            r[0][1] = this->d[0][0] * b[0][1] + this->d[0][1] * b[1][1] + this->d[0][2] * b[2][1];
            r[1][1] = this->d[1][0] * b[0][1] + this->d[1][1] * b[1][1] + this->d[1][2] * b[2][1];
            r[2][1] = this->d[2][0] * b[0][1] + this->d[2][1] * b[1][1] + this->d[2][2] * b[2][1];
            r[0][2] = this->d[0][0] * b[0][2] + this->d[0][1] * b[1][2] + this->d[0][2] * b[2][2];
            r[1][2] = this->d[1][0] * b[0][2] + this->d[1][1] * b[1][2] + this->d[1][2] * b[2][2];
            r[2][2] = this->d[2][0] * b[0][2] + this->d[2][1] * b[1][2] + this->d[2][2] * b[2][2];
            return r;
        }

        void operator*=(const Mat3<FloatType>& b) {
            FloatType temp00 = this->d[0][0] * b[0][0] + this->d[0][1] * b[1][0] + this->d[0][2] * b[2][0];
            FloatType temp10 = this->d[1][0] * b[0][0] + this->d[1][1] * b[1][0] + this->d[1][2] * b[2][0];
            FloatType temp20 = this->d[2][0] * b[0][0] + this->d[2][1] * b[1][0] + this->d[2][2] * b[2][0];
            FloatType temp01 = this->d[0][0] * b[0][1] + this->d[0][1] * b[1][1] + this->d[0][2] * b[2][1];
            FloatType temp11 = this->d[1][0] * b[0][1] + this->d[1][1] * b[1][1] + this->d[1][2] * b[2][1];
            FloatType temp21 = this->d[2][0] * b[0][1] + this->d[2][1] * b[1][1] + this->d[2][2] * b[2][1];
            FloatType temp02 = this->d[0][0] * b[0][2] + this->d[0][1] * b[1][2] + this->d[0][2] * b[2][2];
            FloatType temp12 = this->d[1][0] * b[0][2] + this->d[1][1] * b[1][2] + this->d[1][2] * b[2][2];
            FloatType temp22 = this->d[2][0] * b[0][2] + this->d[2][1] * b[1][2] + this->d[2][2] * b[2][2];
            this->d[0][0] = temp00; this->d[0][1] = temp01; this->d[0][2] = temp02;
            this->d[1][0] = temp10; this->d[1][1] = temp11; this->d[1][2] = temp12;
            this->d[2][0] = temp20; this->d[2][1] = temp21; this->d[2][2] = temp22;
        }

        const Mat34<FloatType> operator*(const Mat34<FloatType>& b) const {
            Mat34<FloatType> r;
            r[0][0] = this->d[0][0] * b[0][0] + this->d[0][1] * b[1][0] + this->d[0][2] * b[2][0];
            r[1][0] = this->d[1][0] * b[0][0] + this->d[1][1] * b[1][0] + this->d[1][2] * b[2][0];
            r[2][0] = this->d[2][0] * b[0][0] + this->d[2][1] * b[1][0] + this->d[2][2] * b[2][0];
            r[0][1] = this->d[0][0] * b[0][1] + this->d[0][1] * b[1][1] + this->d[0][2] * b[2][1];
            r[1][1] = this->d[1][0] * b[0][1] + this->d[1][1] * b[1][1] + this->d[1][2] * b[2][1];
            r[2][1] = this->d[2][0] * b[0][1] + this->d[2][1] * b[1][1] + this->d[2][2] * b[2][1];
            r[0][2] = this->d[0][0] * b[0][2] + this->d[0][1] * b[1][2] + this->d[0][2] * b[2][2];
            r[1][2] = this->d[1][0] * b[0][2] + this->d[1][1] * b[1][2] + this->d[1][2] * b[2][2];
            r[2][2] = this->d[2][0] * b[0][2] + this->d[2][1] * b[1][2] + this->d[2][2] * b[2][2];
            r[0][3] = this->d[0][0] * b[0][3] + this->d[0][1] * b[1][3] + this->d[0][2] * b[2][3];
            r[1][3] = this->d[1][0] * b[0][3] + this->d[1][1] * b[1][3] + this->d[1][2] * b[2][3];
            r[2][3] = this->d[2][0] * b[0][3] + this->d[2][1] * b[1][3] + this->d[2][2] * b[2][3];
            return r;
        }
    private:
    };

    typedef Mat3<float> Mat3f;
    typedef Mat3<double> Mat3d;
}