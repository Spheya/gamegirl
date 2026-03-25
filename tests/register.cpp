#include <ggemu/register.hpp>
#include <gtest/gtest.h>

TEST(Register, ReadWrite8) {
	gg::RegisterContext registers{};
	registers.write(gg::Register::D, 0x67);
	ASSERT_EQ(registers.read(gg::Register::D), 0x67);
}

TEST(Register, ReadWrite16) {
	gg::RegisterContext registers{};
	registers.AF = 0x6769;
	registers.write(gg::Register::HL, 0xCAFE);

	ASSERT_EQ(registers.read(gg::Register::AF), 0x6769);
	ASSERT_EQ(registers.read(gg::Register::HL), 0xCAFE);
	ASSERT_EQ(registers.HL, 0xCAFE);
}

TEST(Register, Composition) {
	gg::RegisterContext registers{};
	registers.write(gg::Register::D, 0x67);
	registers.write(gg::Register::E, 0xAA);

	ASSERT_EQ(registers.read(gg::Register::D), 0x67);
	ASSERT_EQ(registers.DE, 0x67AA);
}
