#include <stdio.h>
#include <math.h>

#pragma once

namespace {

    template <typename FloatType>
    class Vec2 {
    public:

        Vec2() {}

        Vec2(const Vec2& v)
            : x(v.x), y(v.y) {}

        Vec2(Vec2&& v)
            : x(v.x), y(v.y) {
            v.x = 0.0;
            v.y = 0.0;
        }

        Vec2(const FloatType& p)
            : x(p), y(p) {}

        Vec2(FloatType* p)
            : x(p[0]), y(p[1]) {}

        template <typename T1, typename T2>
        Vec2(const T1& x, const T2& y)
            : x(static_cast<FloatType>(x)),
              y(static_cast<FloatType>(y)) {}

        const FloatType length() const { return sqrt(x*x + y*y); }

        const Vec2 normalise() const {
            const FloatType len = sqrt(x*x + y*y);
            return Vec2(x/len,
                        y/len);
        }

        const FloatType operator^(const Vec2& b) const { return x * b.y - y * b.x; }

        Vec2& operator=(const Vec2& v) {
            x = v.x;
            y = v.y;
            return *this;
        }

        Vec2& operator=(Vec2&& v) {
            x = v.x;
            y = v.y;
            v.x = 0.0;
            v.y = 0.0;
            return *this;
        }

        void operator*=(const FloatType& b) { x *= b; y *= b; }

        void operator/=(const FloatType& b) { x /= b; y /= b; }

        void operator+=(const Vec2& b) { x += b.x; y += b.y; }

        void operator+=(const FloatType& b) { x += b; y += b; }

        void operator-=(const Vec2& b) { x -= b.x; y -= b.y; }

        void operator-=(const FloatType& b) { x -= b; y -= b; }

        const Vec2 operator-() const { return Vec2(-x,-y); }

        FloatType& operator[](const int i) {
            if (i < 0 || i > 1)
                throw std::runtime_error("Vector index out of bounds.");

            if (i == 0) return x;
            else return y;
        }

        const FloatType& operator[](const int i) const {
            if (i < 0 || i > 1)
                throw std::runtime_error("Vector index out of bounds.");

            if (i == 0) return x;
            else return y;
        }

        FloatType x = 0.0;
        FloatType y = 0.0;
    };

    template <typename FloatType>
    const bool operator==(const Vec2<FloatType>& a, const Vec2<FloatType>& b) {
        return a.x == b.x && a.y == b.y;
    }

    template <typename FloatType>
    const bool operator!=(const Vec2<FloatType>& a, const Vec2<FloatType>& b) {
        return !(a==b);
    }
    template <typename FloatType>
    std::ostream& operator<<(std::ostream& os, const Vec2<FloatType>& v) {
        os << "[" << v.x << ", " << v.y << "]";
        return os;
    }

    template <typename FloatType>
    std::istream& operator>>(std::istream& is, Vec2<FloatType>& v) {
        is >> v.x;
        is >> v.y;
        return is;
    }

    template <typename FloatType>
    const FloatType operator^(const Vec2<FloatType>& a, const Vec2<FloatType>& b) {
        return a.x * b.y - a.y * b.x;
    }

    template <typename FloatType>
    const FloatType operator*(const Vec2<FloatType>& a, const Vec2<FloatType>& b) {
        return a.x * b.x + a.y * b.y;
    }

    template <typename FloatType, typename T1>
    const Vec2<FloatType> operator*(const T1& b, const Vec2<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec2<FloatType>(v.x * s,
                               v.y * s);
    }

    template <typename FloatType, typename T1>
    const Vec2<FloatType> operator*(const Vec2<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec2<FloatType>(v.x * s,
                               v.y * s);
    }

    template <typename FloatType, typename T1>
    const Vec2<FloatType> operator/(const T1& b, const Vec2<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec2<FloatType>(s / v.x,
                               s / v.y);
    }

    template <typename FloatType, typename T1>
    const Vec2<FloatType> operator/(const Vec2<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec2<FloatType>(v.x / s,
                               v.y / s);
    }

    template <typename FloatType>
    const Vec2<FloatType> operator+(const Vec2<FloatType>& a, const Vec2<FloatType>& b) {
        return Vec2<FloatType>(a.x + b.x,
                               a.y + b.y);
    }

    template <typename FloatType, typename T1>
    const Vec2<FloatType> operator+(const T1& b, const Vec2<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec2<FloatType>(v.x + s,
                               v.y + s);
    }

    template <typename FloatType, typename T1>
    const Vec2<FloatType> operator+(const Vec2<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec2<FloatType>(v.x + s,
                               v.y + s);
    }

    template <typename FloatType>
    const Vec2<FloatType> operator-(const Vec2<FloatType>& a, const Vec2<FloatType>& b) {
        return Vec2<FloatType>(a.x - b.x,
                               a.y - b.y);
    }

    template <typename FloatType, typename T1>
    const Vec2<FloatType> operator-(const T1& b, const Vec2<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec2<FloatType>(v.x - s,
                               v.y - s);
    }

    template <typename FloatType, typename T1>
    const Vec2<FloatType> operator-(const Vec2<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec2<FloatType>(v.x - s,
                               v.y - s);
    }

    typedef Vec2<double> Vec2d;
    typedef Vec2<float> Vec2f;

    template <typename FloatType>
    class Vec3 {
    public:

        Vec3() {}

        Vec3(const Vec3& v)
            : x(v.x), y(v.y), z(v.z) {}

        Vec3(Vec3&& v)
            : x(v.x), y(v.y), z(v.z) {
            v.x = 0.0;
            v.y = 0.0;
            v.z = 0.0;
        }

        Vec3(const FloatType& p)
            : x(p), y(p), z(p) {}

        Vec3(FloatType* p)
            : x(p[0]), y(p[1]), z(p[2]) {}

        Vec3(const Vec2<FloatType>& v, const FloatType& z)
            : x(v.x), y(v.y), z(z) {}

        template <typename T1, typename T2, typename T3>
        Vec3(const T1& x, const T2& y, const T3& z)
            : x(static_cast<FloatType>(x)),
              y(static_cast<FloatType>(y)),
              z(static_cast<FloatType>(z)) {}

        const FloatType length() const {
            return sqrt(x*x + y*y + z*z);
        }

        const Vec3 normalise() const {
            const FloatType len = sqrt(x*x + y*y + z*z);
            return Vec3(x/len,
                        y/len,
                        z/len);
        }

        Vec3& operator=(const Vec3& v) {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }

        Vec3& operator=(Vec3&& v) {
            x = v.x;
            y = v.y;
            z = v.z;
            v.x = 0.0;
            v.y = 0.0;
            v.z = 0.0;
            return *this;
        }

        void operator*=(const FloatType& b) { x *= b; y *= b; z *= b; }

        void operator/=(const FloatType& b) { x /= b; y /= b; z /= b; }

        void operator+=(const Vec3& b) { x += b.x; y += b.y; z += b.z; }

        void operator+=(const FloatType& b) { x += b; y += b; z += b; }

        void operator-=(const Vec3& b) { x -= b.x; y -= b.y; z -= b.z; }

        void operator-=(const FloatType& b) { x -= b; y -= b; z -= b; }

        const Vec3 operator-() const { return Vec3(-x,-y,-z); }

        FloatType& operator[](const int i) {
            if (i == 0) return x;
            if (i == 1) return y;
            else return z;
        }

        const FloatType& operator[](const int i) const {
            if (i == 0) return x;
            if (i == 1) return y;
            else return z;
        }

        FloatType x = 0.0;
        FloatType y = 0.0;
        FloatType z = 0.0;
    };

    typedef Vec3<double> Vec3d;
    typedef Vec3<float> Vec3f;

    template <typename FloatType>
    const bool operator==(const Vec3<FloatType>& a, const Vec3<FloatType>& b) {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    template <typename FloatType>
    const bool operator!=(const Vec3<FloatType>& a, const Vec3<FloatType>& b) {
        return !(a==b);
    }

    template <typename FloatType>
    std::istream& operator>>(std::istream& is, Vec3<FloatType>& v) {
        is >> v.x;
        is >> v.y;
        is >> v.z;
        return is;
    }

    template <typename FloatType>
    std::ostream& operator<<(std::ostream& os, const Vec3<FloatType>& v) {
        os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
        return os;
    }

    template <typename FloatType>
    const Vec3<FloatType> operator^(const Vec3<FloatType>& a, const Vec3<FloatType>& b) {
        const FloatType newX = a.y * b.z - a.z * b.y;
        const FloatType newY = a.z * b.x - a.x * b.z;
        const FloatType newZ = a.x * b.y - a.y * b.x;

        return Vec3<FloatType>(newX, newY, newZ);
    }

    template <typename FloatType>
    const FloatType operator*(const Vec3<FloatType>& a, const Vec3<FloatType>& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    template <typename FloatType, typename T1>
    const Vec3<FloatType> operator*(const T1& b, const Vec3<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec3<FloatType>(v.x * s,
                               v.y * s,
                               v.z * s);
    }

    template <typename FloatType, typename T1>
    const Vec3<FloatType> operator*(const Vec3<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec3<FloatType>(v.x * s,
                               v.y * s,
                               v.z * s);
    }

    template <typename FloatType, typename T1>
    const Vec3<FloatType> operator/(const T1& b, const Vec3<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec3<FloatType>(s / v.x,
                               s / v.y,
                               s / v.z);
    }

    template <typename FloatType, typename T1>
    const Vec3<FloatType> operator/(const Vec3<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec3<FloatType>(v.x / s,
                               v.y / s,
                               v.z / s);
    }

    template <typename FloatType>
    const Vec3<FloatType> operator+(const Vec3<FloatType>& a, const Vec3<FloatType>& b) {
        return Vec3<FloatType>(a.x + b.x,
                               a.y + b.y,
                               a.z + b.z);
    }

    template <typename FloatType, typename T1>
    const Vec3<FloatType> operator+(const T1& b, const Vec3<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec3<FloatType>(v.x + s,
                               v.y + s,
                               v.z + s);
    }

    template <typename FloatType, typename T1>
    const Vec3<FloatType> operator+(const Vec3<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec3<FloatType>(v.x + s,
                               v.y + s,
                               v.z + s);
    }

    template <typename FloatType>
    const Vec3<FloatType> operator-(const Vec3<FloatType>& a, const Vec3<FloatType>& b) {
        return Vec3<FloatType>(a.x - b.x,
                               a.y - b.y,
                               a.z - b.z);
    }

    template <typename FloatType, typename T1>
    const Vec3<FloatType> operator-(const T1& b, const Vec3<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec3<FloatType>(v.x - s,
                               v.y - s,
                               v.z - s);
    }

    template <typename FloatType, typename T1>
    const Vec3<FloatType> operator-(const Vec3<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec3<FloatType>(v.x - s,
                               v.y - s,
                               v.z - s);
    }

    template <typename FloatType>
    class Vec4 {
    public:

        Vec4() {}

        Vec4(const Vec4& v)
            : x(v.x), y(v.y), z(v.z), w(v.w) {}

        Vec4(Vec4&& v)
            : x(v.x), y(v.y), z(v.z), w(v.w) {
            v.x = 0.0;
            v.y = 0.0;
            v.z = 0.0;
            v.w = 0.0;
        }

        Vec4(FloatType* p)
            : x(p[0]), y(p[1]), z(p[2]), w(p[3]) {}

        template <typename T1, typename T2, typename T3, typename T4>
        Vec4(const T1& x, const T2& y, const T3& z, const T4& w)
            : x(static_cast<FloatType>(x)),
              y(static_cast<FloatType>(y)),
              z(static_cast<FloatType>(z)),
              w(static_cast<FloatType>(w)) {}

        Vec4(const Vec3<FloatType>& v, const FloatType& w)
            : x(v.x), y(v.y), z(v.z), w(w) {}

        const FloatType length() const {
            return sqrt(x*x + y*y + z*z + w*w);
        }

        const Vec4 normalise() const {
            FloatType len = length();

            return Vec4(x/len,
                        y/len,
                        z/len,
                        w/len);
        }

        const Vec4 homoNorm() const {
            return Vec4(x / w,
                        y / w,
                        z / w,
                        1.0);
        }

        Vec4& operator=(const Vec4& v) {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            return *this;
        }

        Vec4& operator=(Vec4&& v) {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            v.x = 0.0;
            v.y = 0.0;
            v.z = 0.0;
            v.w = 0.0;
            return *this;
        }

        void operator*=(const FloatType& b) { x *= b; y *= b; z *= b; w *= b; }

        void operator/=(const FloatType& b) { x /= b; y /= b; z /= b; w /= b; }

        void operator+=(const Vec4& b) { x += b.x; y += b.y; z += b.z; w += b.w; }

        void operator+=(const FloatType& b) { x += b; y += b; z += b; w += b; }

        void operator-=(const Vec4& b) { x -= b.x; y -= b.y; z -= b.z; w -= b.w; }

        void operator-=(const FloatType& b) { x -= b; y -= b; z -= b; w -= b; }

        const Vec4 operator-() const { return Vec4(-x,-y,-z, -w); }

        FloatType& operator[](const int i) {
            if (i < 0 || i > 3)
                throw std::runtime_error("Vector index out of bounds.");

            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            return w;
        }

        const FloatType& operator[](const int i) const {
            if (i < 0 || i > 3)
                throw std::runtime_error("Vector index out of bounds.");

            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            return w;
        }

        FloatType x = 0.0;
        FloatType y = 0.0;
        FloatType z = 0.0;
        FloatType w = 1.0;
    };

    typedef Vec4<double> Vec4d;
    typedef Vec4<float> Vec4f;

    template <typename FloatType>
    const bool operator==(const Vec4<FloatType>& a, const Vec4<FloatType>& b) {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w = b.w;
    }

    template <typename FloatType>
    const bool operator!=(const Vec4<FloatType>& a, const Vec4<FloatType>& b) {
        return !(a==b);
    }

    template <typename FloatType>
    std::istream& operator>>(std::istream& is, Vec4<FloatType>& v) {
        is >> v.x;
        is >> v.y;
        is >> v.z;
        is >> v.w;
        return is;
    }

    template <typename FloatType>
    std::ostream& operator<<(std::ostream& os, const Vec4<FloatType>& v) {
        os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
        return os;
    }

    template <typename FloatType>
    const Vec4<FloatType> operator^(const Vec4<FloatType>& a, const Vec4<FloatType>& b) {
        const Vec4<FloatType> c = a.homoNorm();
        const Vec4<FloatType> d = b.homoNorm();
        const FloatType newX = c.y * d.z - c.z * d.y;
        const FloatType newY = c.z * d.x - c.x * d.z;
        const FloatType newZ = c.x * d.y - c.y * d.x;

        return Vec4<FloatType>(newX, newY, newZ, 1.0);
    }

    template <typename FloatType>
    const FloatType operator*(const Vec4<FloatType>& a, const Vec4<FloatType>& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    template <typename FloatType, typename T1>
    const Vec4<FloatType> operator*(const T1& b, const Vec4<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec4<FloatType>(v.x * s,
                               v.y * s,
                               v.z * s,
                               v.w * s);
    }

    template <typename FloatType, typename T1>
    const Vec4<FloatType> operator*(const Vec4<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec4<FloatType>(v.x * s,
                               v.y * s,
                               v.z * s,
                               v.w * s);
    }

    template <typename FloatType, typename T1>
    const Vec4<FloatType> operator/(const T1& b, const Vec4<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec4<FloatType>(s / v.x,
                               s / v.y,
                               s / v.z,
                               s / v.w);
    }

    template <typename FloatType, typename T1>
    const Vec4<FloatType> operator/(const Vec4<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec4<FloatType>(v.x / s,
                               v.y / s,
                               v.z / s,
                               v.w / s);
    }

    template <typename FloatType>
    const Vec4<FloatType> operator+(const Vec4<FloatType>& a, const Vec4<FloatType>& b) {
        return Vec4<FloatType>(a.x + b.x,
                               a.y + b.y,
                               a.z + b.z,
                               a.w + b.w);
    }

    template <typename FloatType, typename T1>
    const Vec4<FloatType> operator+(const T1& b, const Vec4<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec4<FloatType>(v.x + s,
                               v.y + s,
                               v.z + s,
                               v.w + s);
    }

    template <typename FloatType, typename T1>
    const Vec4<FloatType> operator+(const Vec4<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec4<FloatType>(v.x + s,
                               v.y + s,
                               v.z + s,
                               v.w + s);
    }

    template <typename FloatType>
    const Vec4<FloatType> operator-(const Vec4<FloatType>& a, const Vec4<FloatType>& b) {
        return Vec4<FloatType>(a.x - b.x,
                               a.y - b.y,
                               a.z - b.z,
                               a.w - b.w);
    }

    template <typename FloatType, typename T1>
    const Vec4<FloatType> operator-(const T1& b, const Vec4<FloatType>& v) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec4<FloatType>(v.x - s,
                               v.y - s,
                               v.z - s,
                               v.w - s);
    }

    template <typename FloatType, typename T1>
    const Vec4<FloatType> operator-(const Vec4<FloatType>& v, const T1& b) {
        const FloatType s = static_cast<FloatType>(b);
        return Vec4<FloatType>(v.x - s,
                               v.y - s,
                               v.z - s,
                               v.w - s);
    }
}