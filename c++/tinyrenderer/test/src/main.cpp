#include <gtest/gtest.h>

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// TEST(Mat, tranpose_2x2){
//     Mat<int, 2, 2> mat ={
//         Vec<int, 2>{1,10},
//         Vec<int, 2>{4,7},
//     };

//     Mat<int, 2, 2> ans ={
//         Vec<int, 2>{1,4},
//         Vec<int, 2>{10,7},
//     };

//     bool res= (mat.tranpose() == ans);
//     ASSERT_EQ(true, res);

// }

// TEST(Mat, tranpose_3x3){
//     Mat<int, 3, 3> mat ={
//         Vec<int, 3>{1,10,1},
//         Vec<int, 3>{4,7,2},
//         Vec<int, 3>{6,2,1},
//         };

//     Mat<int, 3, 3> ans ={
//         Vec<int, 3>{1,4,6},
//         Vec<int, 3>{10,7,2},
//         Vec<int, 3>{1,2,1},
//         };

//     bool res= (mat.tranpose() == ans);
//     ASSERT_EQ(true, res);

// }