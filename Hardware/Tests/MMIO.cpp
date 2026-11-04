#include <gtest/gtest.h>

#include "../Interface/Mmio.hpp"


using namespace sure;
using namespace sure::hw;


#if SURE__OS_LINUX_

#include <sys/mman.h>


// Linux-only: a real page is mapped at the fixed address the MMIO types point to.
class MMIOTest : public ::testing::Test {
  protected:
    static constexpr Size BASE = 0x20000000;
    static constexpr Size PAGE = 4096;

    Void* _mapped = NULL_PTR;

    void SetUp() override {
        // Without MAP_FIXED the address is only a hint, so an existing mapping is never clobbered.
        Void* p = mmap(reinterpret_cast<Void*>(BASE), PAGE, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        if (p == MAP_FAILED) {
            GTEST_SKIP() << "mmap failed";
        }
        if (p != reinterpret_cast<Void*>(BASE)) {
            munmap(p, PAGE);
            GTEST_SKIP() << "Could not map the page at the fixed test address";
        }
        _mapped = p;
    }

    void TearDown() override {
        if (_mapped != NULL_PTR) {
            munmap(_mapped, PAGE);
        }
    }

    // Raw accessors: they bypass MMIO on purpose, to prepare and inspect memory independently.
    static U8 get8(Size off)  { return *reinterpret_cast<volatile U8*>(BASE + off); }
    static U16 get16(Size off) { return *reinterpret_cast<volatile U16*>(BASE + off); }
    static U32 get32(Size off) { return *reinterpret_cast<volatile U32*>(BASE + off); }

    static void set8(Size off, U8 v)   { *reinterpret_cast<volatile U8*>(BASE + off) = v; }
    static void set16(Size off, U16 v) { *reinterpret_cast<volatile U16*>(BASE + off) = v; }
    static void set32(Size off, U32 v) { *reinterpret_cast<volatile U32*>(BASE + off) = v; }

    static void fill_ff() {
        for (Size i = 0; i < 16; ++i) {
            set8(i, 0xFF);
        }
    }

    using Reg32 = MMIO<U32, BASE>;
    using Reg8  = MMIO<U8,  BASE + 1>;
    using Reg16 = MMIO<U16, BASE + 2>;

    using LowNibble = BitField<U32, 0, 4>;
    using MidByte   = BitField<U32, 8, 8>;
    using Bit5      = BitField<U32, 5, 1>;
};


// ------------------------- read / write -------------------------


TEST_F(MMIOTest, ReadReturnsWholeRegister) {
    set32(0, 0xDEADBEEF);

    EXPECT_EQ(Reg32::read(), 0xDEADBEEF);
}


TEST_F(MMIOTest, WriteStoresWholeRegister) {
    Reg32::write(0x12345678);

    EXPECT_EQ(get32(0), 0x12345678);
}


TEST_F(MMIOTest, WriteThenReadRoundTrips) {
    Reg32::write(0xCAFEBABE);

    EXPECT_EQ(Reg32::read(), 0xCAFEBABE);
}


// ------------------------- access width -------------------------


TEST_F(MMIOTest, U8WriteTouchesOnlyOneByte) {
    fill_ff();

    Reg8::write(0x12);

    EXPECT_EQ(get8(0), 0xFF);
    EXPECT_EQ(get8(1), 0x12);
    EXPECT_EQ(get8(2), 0xFF);
}


TEST_F(MMIOTest, U16WriteTouchesOnlyTwoBytes) {
    fill_ff();

    Reg16::write(0xABCD);

    EXPECT_EQ(get8(1), 0xFF);
    EXPECT_EQ(get16(2), 0xABCD);
    EXPECT_EQ(get8(4), 0xFF);
}


TEST_F(MMIOTest, U32WriteTouchesOnlyFourBytes) {
    fill_ff();

    Reg32::write(0);

    EXPECT_EQ(get32(0), 0u);
    EXPECT_EQ(get8(4), 0xFF);
}


TEST_F(MMIOTest, AdjacentRegistersAreIndependent) {
    using RegA = MMIO<U32, BASE + 0x10>;
    using RegB = MMIO<U32, BASE + 0x14>;

    RegA::write(0x11111111);
    RegB::write(0x22222222);

    EXPECT_EQ(RegA::read(), 0x11111111);
    EXPECT_EQ(RegB::read(), 0x22222222);
}


// ------------------------- read_field -------------------------


TEST_F(MMIOTest, ReadFieldExtractsOnlyTheField) {
    set32(0, 0xAABBCCDD);

    EXPECT_EQ(Reg32::read_field<MidByte>(), 0xCC);
    EXPECT_EQ(Reg32::read_field<LowNibble>(), 0xD);
}


TEST_F(MMIOTest, ReadFieldSingleBit) {
    set32(0, 0x00000020);
    EXPECT_EQ(Reg32::read_field<Bit5>(), 1u);

    set32(0, 0xFFFFFFDF);
    EXPECT_EQ(Reg32::read_field<Bit5>(), 0u);
}


// ------------------------- modify_field -------------------------


TEST_F(MMIOTest, ModifyFieldPreservesOtherBits) {
    set32(0, 0xAABBCCDD);

    Reg32::modify_field<MidByte>(0x11);

    EXPECT_EQ(get32(0), 0xAABB11DD);
}


TEST_F(MMIOTest, ModifyFieldTruncatesValuesThatDoNotFit) {
    set32(0, 0xFFFFFF00);

    Reg32::modify_field<LowNibble>(0xFF);

    EXPECT_EQ(get32(0), 0xFFFFFF0F);
}


TEST_F(MMIOTest, ModifyFieldTwiceKeepsFirstChange) {
    set32(0, 0);

    Reg32::modify_field<LowNibble>(0x5);
    Reg32::modify_field<MidByte>(0x7E);

    EXPECT_EQ(get32(0), 0x00007E05);
}


// ------------------------- set_bits / clear_bits -------------------------


TEST_F(MMIOTest, SetBitsSetsEveryBitOfMultiBitField) {
    set32(0, 0);

    Reg32::set_bits<MidByte>();
    EXPECT_EQ(get32(0), 0x0000FF00);

    Reg32::set_bits<LowNibble>();
    EXPECT_EQ(get32(0), 0x0000FF0F);
}


TEST_F(MMIOTest, SetBitsSingleBit) {
    set32(0, 0);

    Reg32::set_bits<Bit5>();

    EXPECT_EQ(get32(0), 0x00000020);
}


TEST_F(MMIOTest, ClearBitsClearsOnlyTheField) {
    set32(0, 0xFFFFFFFF);

    Reg32::clear_bits<MidByte>();

    EXPECT_EQ(get32(0), 0xFFFF00FF);
}


// ------------------------- access modes -------------------------


TEST_F(MMIOTest, ReadOnlyRegisterSupportsReadAndReadField) {
    using Status = MMIO<U32, BASE + 0x20, MMIOAccessMode::ReadOnly>;
    set32(0x20, 0x0000A500);

    EXPECT_EQ(Status::read(), 0x0000A500);
    EXPECT_EQ(Status::read_field<MidByte>(), 0xA5);
}


TEST_F(MMIOTest, WriteOnlyRegisterSupportsWrite) {
    using Command = MMIO<U32, BASE + 0x24, MMIOAccessMode::WriteOnly>;

    Command::write(0x600DF00D);

    EXPECT_EQ(get32(0x24), 0x600DF00D);
}


// ------------------------- compile-time properties -------------------------


TEST_F(MMIOTest, OperationsAreNoexceptAndTypedByRegister) {
    static_assert(noexcept(Reg32::read()));
    static_assert(noexcept(Reg32::write(0)));
    static_assert(noexcept(Reg32::modify_field<MidByte>(0)));
    static_assert(SameAs<decltype(Reg32::read()), U32>);
    static_assert(SameAs<decltype(Reg16::read()), U16>);
    static_assert(SameAs<decltype(Reg8::read()), U8>);

    SUCCEED();
}


#else // !SURE__OS_LINUX_


TEST(MMIO, HostTestsRequireLinux) {
    GTEST_SKIP() << "MMIO host tests need a fixed-address mapping; implemented for Linux only.";
}


#endif