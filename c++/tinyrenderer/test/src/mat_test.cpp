
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