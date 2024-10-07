#include "tgaimage.h"
namespace Tool
{
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
        int slopeVal = std::abs(dy) * 2;
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

} // namespace Tool