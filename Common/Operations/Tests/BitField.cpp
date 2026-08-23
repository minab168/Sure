#include <gtest/gtest.h>

#include "../BitField.hpp"


using namespace sure::base;


TEST(BitFieldMask, CoversWidthsFromOneToFullRegister) {
    EXPECT_EQ((BitField<U8, 0, 1>::mask()), 0b00000001);
    EXPECT_EQ((BitField<U8, 2, 3>::mask()), 0b00000111);
    EXPECT_EQ((BitField<U8, 0, 8>::mask()), 0b11111111);
    EXPECT_EQ((BitField<U16, 4, 8>::mask()), 0x00FF);
}


TEST(BitFieldWrite, ReplacesOnlySelectedBits) {
    constexpr U8 initial  = 0b10101101;
    constexpr U8 expected = 0b10101001;

    EXPECT_EQ((BitField<U8, 2, 3>::write(initial, 0b010)), expected);
}


TEST(BitFieldWrite, TruncatesValuesThatDoNotFit) {
    EXPECT_EQ((BitField<U8, 1, 3>::write(0, 0b11111111)), 0b00001110);
    EXPECT_EQ((BitField<U8, 0, 8>::write(0, 0xAB)), 0xAB);
}


TEST(BitFieldExtract, ExtractsMultiBitFieldsAtRegisterEdges) {
    EXPECT_EQ((BitField<U8, 0, 3>::extract(0b11100101)), 0b101);
    EXPECT_EQ((BitField<U8, 4, 3>::extract(0b10100101)), 0b010);
    EXPECT_EQ((BitField<U16, 0, 16>::extract(0xBEEF)), 0xBEEF);
}


TEST(BitFieldStatus, ExtractsSingleBitAsStatus) {
    EXPECT_EQ((BitField<U8, 0, 1>::extract(0b00000001)), BitStatus::On);
    EXPECT_EQ((BitField<U8, 0, 1>::extract(0b00000000)), BitStatus::Off);
    EXPECT_EQ((BitField<U8, 7, 1>::extract(0b10000000)), BitStatus::On);
    EXPECT_EQ((BitField<U8, 7, 1>::extract(0b01111111)), BitStatus::Off);
}


TEST(BitFieldValidity, AcceptsOnlyValuesRepresentableByField) {
    EXPECT_TRUE((BitField<U8, 2, 3>::is_valid(0)));
    EXPECT_TRUE((BitField<U8, 2, 3>::is_valid(0b111)));
    EXPECT_FALSE((BitField<U8, 2, 3>::is_valid(0b1000)));
    EXPECT_TRUE((BitField<U8, 0, 8>::is_valid(0xFF)));
}
