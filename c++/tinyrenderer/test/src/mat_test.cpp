
#include <gtest/gtest.h>
#include "geometry.h"


TEST(Mat, tranpose){
    Mat<int, 2, 2> mat ={
        Vec<int, 2>{1,10},
        Vec<int, 2>{4,7},
    };

    Mat<int, 2, 2> ans ={
        Vec<int, 2>{1,4},
        Vec<int, 2>{10,7},
    };

    bool res= (mat.tranpose() == ans);
    ASSERT_EQ(true, res);

}

TEST(Mat, tranpose_3x3){
    Mat<int, 3, 3> mat ={
        Vec<int, 3>{1,10,1},
        Vec<int, 3>{4,7,2},
        Vec<int, 3>{6,2,1},
        };

    Mat<int, 3, 3> ans ={
        Vec<int, 3>{1,4,6},
        Vec<int, 3>{10,7,2},
        Vec<int, 3>{1,2,1},
        };

    bool res= (mat.tranpose() == ans);
    ASSERT_EQ(true, res);

}


TEST(Mat, tranpose_4x4){
    Mat<int, 4, 4> mat ={
        Vec<int, 4>{1,10,1,0},
        Vec<int, 4>{4,7,2,7},
        Vec<int, 4>{6,2,1,6},
        Vec<int, 4>{1,-1,4,8},
        };

    Mat<int, 4, 4> ans ={
        Vec<int, 4>{1,4,6,1},
        Vec<int, 4>{10,7,2,-1},
        Vec<int, 4>{1,2,1,4},
        Vec<int, 4>{0,7,6,8},
        };

    bool res= (mat.tranpose() == ans);
    ASSERT_EQ(true, res);

}


TEST(Mat, mul_vec4f){
    Mat<float, 4, 4> mat ={
        Vec<float, 4>{1,10,1,0},
        Vec<float, 4>{4,7,2,7},
        Vec<float, 4>{6,2,1,6},
        Vec<float, 4>{1,-1,4,8},
        };

    Vec4f vec = {1,2,3,4};
    Vec4f temp = mat*vec;
    Vec4f ans = {24 , 52, 37, 43};

    bool res= (ans==temp);
    ASSERT_EQ(true, res);

}

TEST(Mat, getCol){
    Mat<float, 3, 3> mat ={
        Vec<float, 3>{1,10,11},
        Vec<float, 3>{4,7,2},
        Vec<float, 3>{5,2,2},
        };

    Vec3f col = mat.col(0);
    Vec3f ans = {1,4,5};
    bool res = (ans==col);
    ASSERT_EQ(true, res);
}


TEST(Mat, getRow){
    Mat<float, 2, 2> mat ={
        Vec<float, 2>{1,10},
        Vec<float, 2>{4,7},
        };

    Vec2f row = mat[0];
    Vec2f ans = {1,10};
    bool res = (ans==row);
    ASSERT_EQ(true, res);
}

TEST(Mat, mul_2x2){
    Mat<float, 2, 2> mat ={
        Vec<float, 2>{1,10},
        Vec<float, 2>{4,7},
        };

    Mat<float, 2, 2> mat2 ={
        Vec<float, 2>{-2,0},
        Vec<float, 2>{5,7},
    };

    Mat<float, 2, 2> temp = mat*mat2;

    Mat<float, 2, 2> ans = {
        Vec<float, 2>{48,70},
        Vec<float, 2>{27,49},
    };

    bool res = (ans==temp);
    ASSERT_EQ(true, res);
}