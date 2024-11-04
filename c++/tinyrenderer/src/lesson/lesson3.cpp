#include <iostream>
#include <utility>
#include <vector>
#include "../geometry.h"
#include "../tgaimage.h"
#include "../tools.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);

void rasterize(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color, std::vector<int> &yBuffer)
{
    if (p0.x > p1.x)
    {
        std::swap(p0, p1);
    }

    for (int i = p0.x; i < p1.x; i++)
    {
        float r   = static_cast<float>(i - p0.x) / (p1.x - p0.x);
        Vec2i res = Tool::lerp(r, p0, p1);
        if (res.y > yBuffer[i])
        {
            yBuffer[i] = res.y;
            image.set(i, 0, color);
        }
    }
}

int main(int argc, char **argv)
{
    int      width  = 800;
    int      height = 500;
    TGAImage scene(width, height, TGAImage::RGB);
    // image.set(52, 41, white);
    // scene "2d mesh"  imagine this the side view
    Tool::line5(Vec2i(20, 34), Vec2i(744, 400), scene, red);
    Tool::line5(Vec2i(120, 434), Vec2i(444, 400), scene, green);
    Tool::line5(Vec2i(330, 463), Vec2i(594, 200), scene, blue);
    // screen
    Tool::line5(Vec2i(10, 10), Vec2i(790, 10), scene, white);
    scene.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    scene.write_tga_file("../dist/sideViewOf3Plane.tga");

    // top view
    TGAImage         image(width, 16, TGAImage::RGB);
    std::vector<int> yBuffer(width);
    for (int i = 0; i < width; i++)
    {
        yBuffer[i] = std::numeric_limits<int>::min();
    }
    rasterize(Vec2i(20, 34), Vec2i(744, 400), image, red, yBuffer);
    rasterize(Vec2i(120, 434), Vec2i(444, 400), image, green, yBuffer);
    rasterize(Vec2i(330, 463), Vec2i(594, 200), image, blue, yBuffer);
    /// draw
    image.flip_vertically();
    image.write_tga_file("../dist/topViewOf3Plane.tga");

    std::cout
        << "Success" << std::endl;
    return 0;
}
