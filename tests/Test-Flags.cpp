#include <gtest/gtest.h>

#include "Flags.hpp"

using namespace hpl;

enum FlagEnum {
    F1 = 1 << 0,
    F2 = 1 << 1,
    F3 = 1 << 2
};

TEST(Flags, DefaultConstructor)
{
    Flags<FlagEnum> flags;

    EXPECT_FALSE(flags[F1]);
    EXPECT_FALSE(flags[F2]);
    EXPECT_FALSE(flags[F3]);
}

TEST(Flags, NormalConstructor)
{
    Flags<FlagEnum> flags(F1, F2);

    EXPECT_TRUE(flags[F1]);
    EXPECT_TRUE(flags[F2]);
    EXPECT_FALSE(flags[F3]);
}

TEST(Flags, GetInternals)
{
    Flags<FlagEnum> flags(F1, F2);
    int internal = flags.operator StoredEnum();

    EXPECT_TRUE(internal & F1);
    EXPECT_TRUE(internal & F2);
    EXPECT_FALSE(internal & F3);
}

TEST(Flags, BitwiseChangeFlags)
{
    Flags<FlagEnum> flags(F1);

    ASSERT_TRUE(flags[F1]);
    ASSERT_FALSE(flags[F2]);
    ASSERT_FALSE(flags[F3]);

    flags |= F2;

    EXPECT_TRUE(flags[F1]);
    EXPECT_TRUE(flags[F2]);
    EXPECT_FALSE(flags[F3]);

    flags = flags | F3;

    EXPECT_TRUE(flags[F1]);
    EXPECT_TRUE(flags[F2]);
    EXPECT_TRUE(flags[F3]);

    flags &= F2;

    EXPECT_FALSE(flags[F1]);
    EXPECT_TRUE(flags[F2]);
    EXPECT_FALSE(flags[F3]);

    flags = flags & F1;

    EXPECT_FALSE(flags[F1]);
    EXPECT_FALSE(flags[F2]);
    EXPECT_FALSE(flags[F3]);

    flags ^= F3;

    EXPECT_FALSE(flags[F1]);
    EXPECT_FALSE(flags[F2]);
    EXPECT_TRUE(flags[F3]);

    flags = flags ^ F3;

    EXPECT_FALSE(flags[F1]);
    EXPECT_FALSE(flags[F2]);
    EXPECT_FALSE(flags[F3]);
}

TEST(Flags, OperatorChangeFlags)
{
    Flags<FlagEnum> flags(F1, F2);

    ASSERT_TRUE(flags[F1]);
    ASSERT_TRUE(flags[F2]);
    ASSERT_FALSE(flags[F3]);

    flags(F1, false);
    flags(F3, true);

    EXPECT_FALSE(flags[F1]);
    EXPECT_TRUE(flags[F2]);
    EXPECT_TRUE(flags[F3]);
}

TEST(Flags, Reset)
{
    Flags<FlagEnum> flags(F1, F2);

    ASSERT_TRUE(flags[F1]);
    ASSERT_TRUE(flags[F2]);
    ASSERT_FALSE(flags[F3]);

    flags.reset();

    EXPECT_FALSE(flags[F1]);
    EXPECT_FALSE(flags[F2]);
    EXPECT_FALSE(flags[F3]);
}

TEST(Flags, Invert)
{
    Flags<FlagEnum> flags(F1, F2);

    ASSERT_TRUE(flags[F1]);
    ASSERT_TRUE(flags[F2]);
    ASSERT_FALSE(flags[F3]);

    flags.invert();

    EXPECT_FALSE(flags[F1]);
    EXPECT_FALSE(flags[F2]);
    EXPECT_TRUE(flags[F3]);

    flags.invert(F2);

    EXPECT_FALSE(flags[F1]);
    EXPECT_TRUE(flags[F2]);
    EXPECT_TRUE(flags[F3]);
}

TEST(Flags, HighOrderGetter)
{
    Flags<FlagEnum> flags(F1, F2);

    EXPECT_TRUE(flags.any());
    EXPECT_FALSE(flags.none());

    //! @todo make these three not compile!
    /*Flags<FlagEnum> f(2);
    f[2];
    f(2);*/
}
