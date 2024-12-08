#include <iostream>
#include <utility>
#include <vector>
#include "../geometry.h"
#include "../model.h"
#include "../tgaimage.h"
#include "../tools.h"
#include <expected>
#include <string_view>


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

    int value = __cplusplus;
    std::cout<<value<<std::endl;


    int      width  = 800;
    int      height = 500;
    TGAImage scene(width, height, TGAImage::RGB);

    { // draw the axes
        Vec3f x(1.f, 0.f, 0.f), y(0.f, 1.f, 0.f), o(0.f, 0.f, 0.f);
    }

    Mat<int, 3, 3> mat ={
        Vec<int, 3>{1,10,1},
        Vec<int, 3>{4,7,2},
        Vec<int, 3>{6,2,1},
        };

    std::cout<<mat.det();
    //std::cout<<mat2by2.det();
    //std::cout<<mat2by2.det();

    return 0;
}