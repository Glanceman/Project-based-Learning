#include <gtest/gtest.h>
#include "geometry.h"


TEST(Vec3, dot){
    Vec2i a(20, 34);
    Vec2i b(1, -4);

    Vec2i ans(21,30);

    bool res= ( (a+b) == ans);
    ASSERT_EQ(true, res);
}