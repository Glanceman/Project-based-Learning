#include <iostream>
#include "../geometry.h"
#include "../tgaimage.h"
#include "../tools.h"
#include "../model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);

int main(int argc, char **argv)
{
    TGAImage image(100, 100, TGAImage::RGB);
    // image.set(52, 41, white);
    Tool::line(10, 10, 80, 80, image, white);
    Tool::line2(10, 20, 80, 90, image, red);
    Tool::line3(10, 30, 80, 100, image, red);
    Tool::line4(10, 40, 80, 110, image, white);
    Tool::line5(10, 50, 80, 120, image, white);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("../dist/output.tga");

    // import the model
    int      width  = 800;
    int      height = 800;
    TGAImage image1(width, height, TGAImage::RGB);
    Model    model("../asset/african_head.obj");

    // draw the wireframe of the model
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<int> face = model.face(i); // 1193/1240/1193
        for (int j = 0; j < 3; j++)
        {
            Vec3f v0 = model.vert(face[j]);
            Vec3f v1 = model.vert(face[(j + 1) % 3]);
            int   x0 = (v0.x + 1.) * width / 2.;
            int   y0 = (v0.y + 1.) * height / 2.;
            int   x1 = (v1.x + 1.) * width / 2.;
            int   y1 = (v1.y + 1.) * height / 2.;
            Tool::line4(x0, y0, x1, y1, image1, red);
        }
    }
    // save the image
    image1.flip_vertically();
    image1.write_tga_file("../dist/model.tga");
    std::cout
        << "Success" << std::endl;
    return 0;
}
