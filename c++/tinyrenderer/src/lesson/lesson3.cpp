#include <iostream>
#include "../geometry.h"
#include "../tgaimage.h"
#include "../tools.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);

int main(int argc, char **argv)
{
    TGAImage image(800, 500, TGAImage::RGB);
    // image.set(52, 41, white);
    // scene "2d mesh"
    Tool::line5(Vec2i(20, 34), Vec2i(744, 400), image, red);
    Tool::line5(Vec2i(120, 434), Vec2i(444, 400), image, green);
    Tool::line5(Vec2i(330, 463), Vec2i(594, 200), image, blue);

    Tool::line5(Vec2i(10, 10), Vec2i(790, 10), image, white);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("../dist/Depth.tga");

    std::cout
        << "Success" << std::endl;
    return 0;
}
