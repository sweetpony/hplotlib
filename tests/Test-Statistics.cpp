#include <cmath>

#include <gtest/gtest.h>

#include "Statistics.hpp"

using namespace hpl;

TEST(Statistics, minmax)
{
    double x[4] = {-2.0, 0.0, 1.0, 4.0};
    EXPECT_EQ(-2.0, min(4, x));
    EXPECT_EQ(4.0, max(4, x));
    EXPECT_EQ(1.0, minPos(4, x));
    EXPECT_EQ(4.0, maxPos(4, x));
}

TEST(Statistics, interpolate)
{
    EXPECT_EQ(8.0, interpolate(2.0, 1.0, 5.0, 5.0, 17.0));
}

TEST(Statistics, log)
{
    double x[4] = {-2.0, 0.0, 1.0, 1000.0};
    double* xl = log(5.0, x);

    EXPECT_FALSE(std::isfinite(xl[0]));
    EXPECT_FALSE(std::isfinite(xl[1]));
    ASSERT_TRUE(std::isfinite(xl[2]));
    EXPECT_EQ(0.0, xl[2]);
    ASSERT_TRUE(std::isfinite(xl[3]));
    EXPECT_EQ(3.0, xl[3]);

    delete[] xl;
}
