#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cassert>
#include <cmath>
#include <ostream>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, int n>
struct Vec
{
    T data[n] = {0};

    T &operator[](const int i)
    {
        assert(i >= 0 && i < n);
        return data[i];
    }

    T operator[](const int i) const
    {
        assert(i >= 0 && i < n);
        return data[i];
    }

    T norm2() const { return *this * *this; }
    T norm() const { return std::sqrt(norm2()); }
};

template <typename T, int n>
Vec<T, n> operator+(const Vec<T, n> &lhs, const Vec<T, n> &rhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] += rhs[i]);
    return ret;
}

template <typename T, int n>
Vec<T, n> operator-(const Vec<T, n> &lhs, const Vec<T, n> &rhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] -= rhs[i]);
    return ret;
}

template <typename T, int n>
T operator*(const Vec<T, n> &lhs, const Vec<T, n> &rhs)
{
    T ret = 0;
    for (int i = n; i--; ret += lhs[i] * rhs[i]);
    return ret;
}

template <typename T, int n>
Vec<T, n> operator*(const T &rhs, const Vec<T, n> &lhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] *= rhs);
    return ret;
}

template <typename T, int n>
Vec<T, n> operator*(const Vec<T, n> &lhs, const T &rhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] *= rhs);
    return ret;
}

template <typename T, int n>
Vec<T, n> operator/(const Vec<T, n> &lhs, const T &rhs)
{
    Vec<T, n> ret = lhs;
    for (int i = n; i--; ret[i] /= rhs);
    return ret;
}

template <typename T, int n1, int n2>
Vec<T, n1> embed(const Vec<T, n2> &v, T fill = 1)
{
    Vec<T, n1> ret;
    for (int i = n1; i--; ret[i] = (i < n2 ? v[i] : fill));
    return ret;
}

template <typename T, int n1, int n2>
Vec<T, n1> proj(const Vec<T, n2> &v)
{
    Vec<T, n1> ret;
    for (int i = n1; i--; ret[i] = v[i]);
    return ret;
}

template <typename T, int n>
std::ostream &operator<<(std::ostream &out, const Vec<T, n> &v)
{
    for (int i = 0; i < n; i++) out << v[i] << " ";
    return out;
}

template <typename T>
struct Vec<T, 2>
{
    union
    {
        struct
        {
            T u, v;
        };
        struct
        {
            T x, y;
        };
    };

    constexpr Vec<T, 2>() noexcept :
        u(0), v(0) {}

    constexpr Vec<T, 2>(T _u, T _v) noexcept :
        u(_u), v(_v) {}

    T &operator[](const int i)
    {
        assert(i >= 0 && i < 2);
        return i ? y : x;
    }

    T operator[](const int i) const
    {
        assert(i >= 0 && i < 2);
        return i ? y : x;
    }

    inline T         norm2() const { return *this * *this; }
    inline T         norm() const { return std::sqrt(norm2()); }
    inline Vec<T, 2> normalized() { return (*this) / norm(); }
    inline T         dot(const Vec<T, 2> &vec) const { return *this * vec; }
    inline Vec<T, 2> dot(T scalar) const { return *this * scalar; }
    inline T         cross(const Vec<T, 2> &vec) const { return ((*this).u * vec.v - (*this).v * vec.u); }
};

template <typename T>
struct Vec<T, 3>
{
    T x = 0, y = 0, z = 0;

    constexpr Vec<T, 3>() noexcept = default;

    constexpr Vec<T, 3>(T _x, T _y, T _z) noexcept :
        x(_x), y(_y), z(_z) {}

    T &operator[](const int i)
    {
        assert(i >= 0 && i < 3);
        return i ? (1 == i ? y : z) : x;
    }

    T operator[](const int i) const
    {
        assert(i >= 0 && i < 3);
        return i ? (1 == i ? y : z) : x;
    }

    inline T         norm2() const { return *this * *this; }
    inline T         norm() const { return std::sqrt(norm2()); }
    inline Vec<T, 3> normalized() { return (*this) / norm(); }
    inline T         dot(const Vec<T, 3> &vec) const { return *this * vec; }
    inline Vec<T, 3> cross(const Vec<T, 3> &v) const
    {
        return Vec<T, 3>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

// Type definitions with default type double
using Vec2  = Vec<double, 2>;
using Vec2f = Vec<float, 2>;
using Vec2i = Vec<int, 2>;
using Vec3  = Vec<double, 3>;
using Vec3f = Vec<float, 3>;
using Vec3i = Vec<int, 3>;
using Vec4  = Vec<double, 4>;
using Vec4f = Vec<float, 4>;
using Vec4i = Vec<int, 4>;

#endif //__GEOMETRY_H__