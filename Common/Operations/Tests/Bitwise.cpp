#include <gtest/gtest.h>

#include "../Bitwise.hpp"


TEST(BitwiseMask, SetGetClearAndToggle) {
    constexpr U8 value = 0b00001100;
    constexpr U8 mask = 0b00000110;

    EXPECT_EQ(bit_set_mask(value, mask), 0b00001110);
    EXPECT_EQ(bit_get_mask(value, mask), 0b00000100);
    EXPECT_EQ(bit_clear_mask(value, mask), 0b00001000);
    EXPECT_EQ(bit_toggle_mask(value, mask), 0b00001010);
}


TEST(BitwiseSingleBit, SupportsLowestAndHighestPositions) {
    constexpr U8 highest = sizeof(U8) * CHAR_SIZE_ - 1;

    EXPECT_EQ(bit_set<U8>(0, 0), 0b00000001);
    EXPECT_EQ(bit_set<U8>(0, highest), 0b10000000);
    EXPECT_EQ(bit_get<U8>(0b10000001, 0), 1);
    EXPECT_EQ(bit_get<U8>(0b10000001, highest), 1);
    EXPECT_TRUE(bit_check<U8>(0b10000001, 0));
    EXPECT_FALSE(bit_check<U8>(0b10000001, 1));
    EXPECT_EQ(bit_clear<U8>(0b10000001, 0), 0b10000000);
    EXPECT_EQ(bit_toggle<U8>(0b10000001, highest), 0b00000001);
}


TEST(BitwiseShift, PreservesTypeAndHandlesZeroShift) {
    constexpr U16 value = 0x1234;

    EXPECT_EQ(bit_lsh(value, 0), value);
    EXPECT_EQ(bit_rsh(value, 0), value);
    EXPECT_EQ(bit_lsh<U16>(1, 15), 0x8000);
    EXPECT_EQ(bit_rsh<U16>(0x8000, 15), 1);
}


TEST(BitwiseRotate, NormalizesShiftAndHandlesFullWidth) {
    constexpr U8 value = 0b10000001;

    EXPECT_EQ(rotl<U8>(value, 0), value);
    EXPECT_EQ(rotl<U8>(value, 8), value);
    EXPECT_EQ(rotl<U8>(value, 1), 0b00000011);
    EXPECT_EQ(rotr<U8>(value, 0), value);
    EXPECT_EQ(rotr<U8>(value, 8), value);
    EXPECT_EQ(rotr<U8>(value, 1), 0b11000000);
}
