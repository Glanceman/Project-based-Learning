#include "geometry.h"
#include "tgaimage.h"
#include <array>
#include <iostream>
#include <utility>
#include <algorithm>
namespace Tool
{

    inline Vec3f WorldToScreen(Vec3f v, TGAImage &screen)
    {
        int width  = screen.get_width();
        int height = screen.get_height();
        return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
    }

    inline Vec2f lerp(float r, Vec2f p0, Vec2f p1)
    {
        return p0 + (p1 - p0).dot(r);
    }

    inline Vec2i lerp(float r, Vec2i p0, Vec2i p1)
    {
        return p0.dot(1 - r) + (p1).dot(r);
    }

    /// @brief version 1 bad if we choose large stepping, small will be example
    /// @param x0
    /// @param y0
    /// @param x1
    /// @param y1
    /// @param image
    /// @param color
    inline void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
    {
        for (float t = 0.; t < 1.; t += .01)
        {
            int x = x0 + (x1 - x0) * t;
            int y = y0 + (y1 - y0) * t;
            image.set(x, y, color);
        }
    }

    /// @brief verison 2 auto stepping but only consider 1 direction
    /// @param x0
    /// @param y0
    /// @param x1
    /// @param y1
    /// @param image
    /// @param color
    inline void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
    {
        for (int x = x0; x <= x1; x++)
        {
            float t = (x - x0) / (float)(x1 - x0);
            int   y = y0 * (1. - t) + y1 * t;
            image.set(x, y, color);
        }
    }

    /// @brief line3 prevent divide 0 by swap x and y
    /// @param x0
    /// @param y0
    /// @param x1
    /// @param y1
    /// @param image
    /// @param color
    inline void line3(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
    {
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1))
        { // if the line is steep >45, we transpose the image
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1)
        { // make it left−to−right
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        for (int x = x0; x <= x1; x++)
        {
            float t = (x - x0) / (float)(x1 - x0);
            int   y = y0 * (1. - t) + y1 * t;
            if (steep)
            {
                image.set(y, x, color); // if transposed, de−transpose
            }
            else
            {
                image.set(x, y, color);
            }
        }
    }

    /// @brief fix version 3 error
    /// @param x0
    /// @param y0
    /// @param x1
    /// @param y1
    /// @param image
    /// @param color
    inline void line4(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
    {
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1))
        {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int   dx       = x1 - x0;
        int   dy       = y1 - y0;
        float slopeVal = std::abs(dy / float(dx)); //(0 - 1)
        float error    = 0;
        int   y        = y0;

        for (int x = x0; x <= x1; x++)
        {
            if (steep)
            {
                image.set(y, x, color);
            }
            else
            {
                image.set(x, y, color);
            }
            error += slopeVal;
            if (error > .5)
            {
                y += (y1 > y0 ? 1 : -1);
                error -= 1.;
            }
        }
    }

    /// @brief remove division and float
    /// @param x0
    /// @param y0
    /// @param x1
    /// @param y1
    /// @param image
    /// @param color
    inline void line5(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
    {
        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1))
        {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx       = x1 - x0;
        int dy       = y1 - y0;
        int slopeVal = std::abs(dy) * 2; //!!!!!
        int error    = 0;
        int y        = y0;

        for (int x = x0; x <= x1; x++)
        {
            if (steep)
            {
                image.set(y, x, color);
            }
            else
            {
                image.set(x, y, color);
            }
            error += slopeVal;
            if (error > dx)
            {
                y += (y1 > y0 ? 1 : -1);
                error -= dx * 2;
            }
        }
    }

    inline void line5(Vec2i p, Vec2i p1, TGAImage &image, TGAColor color)
    {
        line5(p.x, p.y, p1.x, p1.y, image, color);
    }

    inline void triangle_v1(const Vec2i &v0, const Vec2i &v1, const Vec2i &v2, TGAImage &image, const TGAColor &color)
    {
        line5(v0.x, v0.y, v1.x, v1.y, image, color);
        line5(v1.x, v1.y, v2.x, v2.y, image, color);
        line5(v2.x, v2.y, v0.x, v0.y, image, color);
    }

    // scanline
    inline void triangle_v2(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage &image, const TGAColor &color, bool outline = true)
    {
        // sort by y v0<v1<v2
        if (v0.y > v1.y) std::swap(v0, v1);
        if (v1.y > v2.y) std::swap(v1, v2);
        if (v0.y > v2.y) std::swap(v0, v2);

        // notice not y/x -> x/y
        float slopeA = static_cast<float>(v2.x - v0.x) / static_cast<float>(v2.y - v0.y);
        float slopeB = static_cast<float>(v1.x - v0.x) / static_cast<float>(v1.y - v0.y);
        float slopeC = static_cast<float>(v2.x - v1.x) / static_cast<float>(v2.y - v1.y);

        float diff  = slopeB;
        float curX1 = v0.x;
        float curX2 = v0.x;

        for (int i = v0.y; i <= v2.y; i++)
        {
            if (i >= v1.y)
            {
                // recalculate slope value
                diff = slopeC;
            }
            line5(curX1, i, curX2, i, image, color);
            curX1 = curX1 + slopeA;
            curX2 += diff;
        }

        if (outline)
        {
            line5(v0.x, v0.y, v1.x, v1.y, image, TGAColor(255, 255, 255, 255));
            line5(v1.x, v1.y, v2.x, v2.y, image, TGAColor(255, 255, 255, 255));
            line5(v2.x, v2.y, v0.x, v0.y, image, TGAColor(255, 255, 255, 255));
        }
    }

    // convert Cartesian coordinates into barycentric
    inline Vec3f barycentric(const std::array<Vec2i, 3> &points, const Vec2i &p)
    {
        float area1 = 0.5 * (points[1] - points[0]).cross((p - points[0])); // tri area in clockwise with cross product
        float area2 = 0.5 * (points[2] - points[1]).cross((p - points[1]));
        float area3 = 0.5 * (p - points[0]).cross((points[2] - points[0]));

        float triArea = 0.5 * (points[1] - points[0]).cross((points[2] - points[0]));

        return Vec3f(area1 / triArea, area2 / triArea, area3 / triArea);
    }

    inline Vec3f barycentric(const std::array<Vec2f, 3> &points, const Vec2f &p)
    {
        float area1 = 0.5 * (points[1] - points[0]).cross((p - points[0])); // tri area in clockwise with cross product
        float area2 = 0.5 * (points[2] - points[1]).cross((p - points[1]));
        float area3 = 0.5 * (p - points[0]).cross((points[2] - points[0]));

        float triArea = 0.5 * (points[1] - points[0]).cross((points[2] - points[0]));

        return Vec3f(area1 / triArea, area2 / triArea, area3 / triArea);
    }

    // barycentric coordinates https://www.youtube.com/watch?v=HYAgJN3x4GA&t=29s
    inline void triangle_v3(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage &image, const TGAColor &color, bool outline = true)
    {
        std::array<Vec2i, 3> points = {v0, v1, v2};
        Vec2i                bboxmin(image.get_width() - 1, image.get_height() - 1);
        Vec2i                bboxmax(0, 0);

        for (int i = 0; i < 3; i++)
        {
            bboxmin.x = std::max(0, std::min(points[i].x, bboxmin.x));
            bboxmin.y = std::max(0, std::min(points[i].y, bboxmin.y));
            bboxmax.x = std::min(image.get_width() - 1, std::max(bboxmax.x, points[i].x));
            bboxmax.y = std::min(image.get_height() - 1, std::max(bboxmax.y, points[i].y));
        }

        Vec2i p(bboxmin);

        for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++)
        {
            for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++)
            {
                Vec3f res = barycentric(points, p);
                if (res.x < 0 || res.y < 0 || res.z < 0) continue;
                image.set(p.x, p.y, color);
            }
        }

        if (outline)
        {
            line5(v0.x, v0.y, v1.x, v1.y, image, TGAColor(255, 50, 50, 255));
            line5(v1.x, v1.y, v2.x, v2.y, image, TGAColor(255, 50, 50, 255));
            line5(v2.x, v2.y, v0.x, v0.y, image, TGAColor(255, 50, 50, 255));
        }
    }

    // barycentric coordinates https://www.youtube.com/watch?v=HYAgJN3x4GA&t=29s
    inline void triangle_v4(Vec3f v0, Vec3f v1, Vec3f v2, TGAImage &image, float zBuffer[], const TGAColor &color, bool outline = true)
    {
        std::array<Vec3f, 3> points = {v0, v1, v2};

        Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
        Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

        for (int i = 0; i < 3; i++)
        {
            bboxmin.x = std::max(0.f, std::min(points[i].x, bboxmin.x));
            bboxmin.y = std::max(0.f, std::min(points[i].y, bboxmin.y));
            bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, points[i].x));
            bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, points[i].y));
        }

        Vec3f p(bboxmin.x, bboxmin.y, 0);
        int   img_width  = image.get_width();
        int   img_height = image.get_height();

        std::array<Vec2f, 3> screen_pts;
        for (int i = 0; i < screen_pts.size(); i++)
        {
            screen_pts[i] = Vec2f(points[i].x, points[i].y);
        }

        for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++)
        {
            for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++)
            {
                Vec3f res = barycentric(screen_pts, Vec2f(p.x, p.y));
                if (res.x < 0 || res.y < 0 || res.z < 0) continue;

                // get the z value of the point from
                p.z = 0;
                for (int i = 0; i < 3; i++) // barycentric(x+y+z=1)  (p=p1x+p2y+p3z)
                {
                    p.z += points[i].z * res[i];
                }

                int zIndex = p.y * img_width + p.x;
                if (zBuffer[zIndex] < p.z)
                {
                    image.set(p.x, p.y, color);
                    zBuffer[zIndex] = p.z;
                }
            }
        }

        if (outline)
        {
            line5(v0.x, v0.y, v1.x, v1.y, image, TGAColor(255, 50, 50, 255));
            line5(v1.x, v1.y, v2.x, v2.y, image, TGAColor(255, 50, 50, 255));
            line5(v2.x, v2.y, v0.x, v0.y, image, TGAColor(255, 50, 50, 255));
        }
    }

} // namespace Tool