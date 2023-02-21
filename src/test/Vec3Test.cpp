#include "../utils/Vec3.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

using namespace std;

namespace AllZero {
    Vec3 v = Vec3(0, 0, 0);

    TEST(equalTest, allZero)
    {
        EXPECT_TRUE(v == Vec3::zero());
        EXPECT_TRUE(v == Vec3(0, 0, 0));
        EXPECT_FALSE(v == Vec3(1, 0, 0));
        EXPECT_FALSE(v == Vec3(0, 1, 0));
        EXPECT_FALSE(v == Vec3(0, 0, 1));
    }

    TEST(notEqualTest, allZero)
    {
        EXPECT_TRUE(v != Vec3(1, 0, 0));
        EXPECT_TRUE(v != Vec3(0, 1, 0));
        EXPECT_TRUE(v != Vec3(0, 0, 1));
        EXPECT_FALSE(v != Vec3(0, 0, 0));
    }

    TEST(operationTest, allZero)
    {
        EXPECT_EQ(-v, Vec3(0, 0, 0));
        EXPECT_EQ(v + Vec3(0, 0, 0), Vec3(0, 0, 0));
        EXPECT_EQ(v - Vec3(0, 0, 0), Vec3(0, 0, 0));

        vector<Vec3> x = { Vec3(0, 0, 0), Vec3(1, 2, 3), Vec3(-1, -2, -3), Vec3(0.5, 0.2, -0.3) };
        for (int i = 0; i < x.size(); i++) {
            Vec3 u = v;
            u += Vec3::zero();
            EXPECT_EQ()
        }
    }
}

namespace AllPositive {
    Vec3 v = Vec3(1, 2, 3);

    TEST(equalTest, allPositive)
    {
        EXPECT_TRUE(v == Vec3(1, 2, 3)); // 同じ値の別のオブジェクト
        EXPECT_TRUE(v == v);             // 自分自身
        EXPECT_FALSE(v == Vec3(0, 0, 0));
        EXPECT_FALSE(v == Vec3(-1, -2, -3));
        EXPECT_FALSE(v == Vec3(1, 1, 1));
    }

    TEST(notEqualTest, allPositive)
    {
        EXPECT_FALSE(v != Vec3(1, 2, 3));
        EXPECT_FALSE(v != v);
        EXPECT_TRUE(v != Vec3(0, 0, 0));
        EXPECT_TRUE(v != Vec3(-1, -2, -3));
        EXPECT_TRUE(v != Vec3(1, 1, 1));
    }
}