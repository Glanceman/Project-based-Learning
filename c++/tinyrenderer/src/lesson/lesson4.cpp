#include <iostream>
#include <utility>
#include <vector>
#include "../geometry.h"
#include "../model.h"
#include "../tgaimage.h"
#include "../tools.h"

const TGAColor white  = TGAColor(255, 255, 255, 255);
const TGAColor red    = TGAColor(255, 0, 0, 255);
const TGAColor green  = TGAColor(0, 255, 0, 255);
const TGAColor blue   = TGAColor(0, 0, 255, 255);
const TGAColor yellow = TGAColor(255, 255, 0, 255);

Model    *model  = NULL;
const int width  = 100;
const int height = 100;
const int depth  = 255;

int main(int argc, char **argv)
{
    int      width  = 800;
    int      height = 500;
    TGAImage scene(width, height, TGAImage::RGB);

    return 0;
}