#include "../utils/Vec3.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <iostream>
#include <numbers>
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

        // operator+=
        for (int i = 0; i < x.size(); i++) {
            Vec3 u = v;
            u += x[i];
            EXPECT_EQ(u, x[i]);
        }

        // operator-=
        for (int i = 0; i < x.size(); i++) {
            Vec3 u = v;
            u -= x[i];
            EXPECT_EQ(u, -x[i]);
        }
    }

    TEST(timesScalarTest, allZero)
    {
        for (int i = -5; i <= 5; i++) {
            EXPECT_EQ(v.timesScalar(i), Vec3::zero());
        }
    }

    TEST(dotTest, allZero)
    {
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                for (int z = -5; z <= 5; z++) {
                    EXPECT_EQ(v.dot(Vec3(x, y, z)), 0);
                }
            }
        }
    }

    TEST(rotateTest, allZero)
    {
        for (double theta = -5; theta <= 5; theta += 0.1) {
            for (double phi = -5; phi <= 5; phi += 0.1) {
                EXPECT_EQ(v.rotate(theta, phi), Vec3::zero());
            }
        }
    }

    TEST(crossTest, allZero)
    {
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                for (int z = -5; z <= 5; z++) {
                    EXPECT_EQ(v.cross(Vec3(x, y, z)), Vec3::zero());
                }
            }
        }
    }

    TEST(normalizeTest, allZero)
    {
        EXPECT_EQ(v.normalize(), Vec3::zero());
    }

    TEST(lengthTest, allZero)
    {
        EXPECT_EQ(v.length(), 0);
    }

    TEST(distTest, allZero)
    {
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                for (int z = -5; z <= 5; z++) {
                    EXPECT_EQ(v.dist(Vec3(x, y, z)), Vec3(x, y, z).length());
                }
            }
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

    TEST(operationTest, allPositive)
    {
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                for (int z = -5; z <= 5; z++) {
                    EXPECT_EQ(-Vec3(x, y, z), Vec3(-x, -y, -z));
                    EXPECT_EQ(v + Vec3(x, y, z), Vec3(v.x + x, v.y + y, v.z + z));
                    EXPECT_EQ(v - Vec3(x, y, z), Vec3(v.x - x, v.y - y, v.z - z));

                    Vec3 u = v;
                    u += Vec3(x, y, z);
                    EXPECT_EQ(u, Vec3(v.x + x, v.y + y, v.z + z));

                    u = v;
                    u -= Vec3(x, y, z);
                    EXPECT_EQ(u, Vec3(v.x - x, v.y - y, v.z - z));
                }
            }
        }
    }

    TEST(timesScalarTest, allPositive)
    {
        for (int i = -5; i <= 5; i++) {
            EXPECT_EQ(v.timesScalar(i), Vec3(v.x * i, v.y * i, v.z * i));
        }
    }

    TEST(dotTest, allPositive)
    {
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                for (int z = -5; z <= 5; z++) {
                    EXPECT_EQ(v.dot(Vec3(x, y, z)), v.x * x + v.y * y + v.z * z);
                }
            }
        }
    }

    // TEST(rotateTest, allPositive)
    // {
    //     vector<pair<double, double>> rad = { make_pair(0, 0), make_pair(std::numbers::pi, std::numbers::pi) };
    //     vector<Vec3> res                 = { v, Vec3(v.x, -v.y, v.z) };

    //     for (int i = 0; i < res.size(); i++) {
    //         v.print();
    //         v.rotate(rad[i].first, rad[i].second).print();
    //         EXPECT_EQ(v.rotate(rad[i].first, rad[i].second), res[i]);
    //     }
    // }

    TEST(crossTest, AllPositive)
    {
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                for (int z = -5; z <= 5; z++) {
                    EXPECT_EQ(v.cross(Vec3(x, y, z)), Vec3(v.y * z - v.z * y, v.z * x - v.x * z, v.x * y - v.y * x));
                }
            }
        }
    }

    TEST(normalizeTest, allPositive)
    {
        for (int x = 0; x <= 5; x++) {
            for (int y = 0; y <= 5; y++) {
                for (int z = 0; z <= 5; z++) {
                    double len = Vec3(x, y, z).length();
                    if (len == 0) {
                        continue;
                    }
                    EXPECT_EQ(Vec3(x, y, z).normalize(), Vec3(x / len, y / len, z / len));
                }
            }
        }
    }

    TEST(lengthTest, allPositive)
    {
        for (int x = 0; x <= 5; x++) {
            for (int y = 0; y <= 5; y++) {
                for (int z = 0; z <= 5; z++) {
                    double len = Vec3(x, y, z).length();
                    EXPECT_EQ(Vec3(x, y, z).length(), sqrt(x * x + y * y + z * z));
                }
            }
        }
    }

    TEST(distTest, allPositive)
    {
        for (int x = -5; x <= 5; x++) {
            for (int y = -5; y <= 5; y++) {
                for (int z = -5; z <= 5; z++) {
                    Vec3 diff = v - Vec3(x, y, z);
                    EXPECT_EQ(v.dist(Vec3(x, y, z)), diff.length());
                }
            }
        }
    }
}