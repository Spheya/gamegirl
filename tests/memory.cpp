#include <ggemu/memory.hpp>
#include <gtest/gtest.h>

TEST(Memory, ReadWrite) {
	gg::Memory mem;
	mem.write(0xC0F3, 67); // wram
	mem.write(0xFFFA, 42); // hram
	EXPECT_EQ(mem.read(0xC0F3), 67);
	EXPECT_EQ(mem.read(0xFFFA), 42);
}

TEST(Memory, MoveConstructor) {
	gg::Memory mem;
	mem.write(0xC0F3, 67);

	gg::Memory mem2 = std::move(mem);
	EXPECT_EQ(mem2.read(0xC0F3), 67);
}

TEST(Memory, MoveAssignment) {
	gg::Memory mem;
	mem.write(0xC0F3, 67);

	gg::Memory mem2;
	mem2 = std::move(mem);

	EXPECT_EQ(mem2.read(0xC0F3), 67);
}

TEST(Memory, CopyToBuffer) {
	gg::Memory mem;
	mem.write(0xC000, 10);
	mem.write(0xC001, 20);
	mem.write(0xC002, 30);

	uint8_t buffer[3] = {};
	mem.copy(buffer, 0xC000, 3);

	EXPECT_EQ(buffer[0], 10);
	EXPECT_EQ(buffer[1], 20);
	EXPECT_EQ(buffer[2], 30);
}

TEST(Memory, CopyToMemory) {
	gg::Memory mem;
	uint8_t buffer[3] = { 10, 20, 30 };
	mem.copy(0, buffer, 3);

	EXPECT_EQ(mem.read(0), 10);
	EXPECT_EQ(mem.read(1), 20);
	EXPECT_EQ(mem.read(2), 30);
}

TEST(Memory, ZeroInitialized) {
	gg::Memory mem;
	EXPECT_EQ(mem.read(0xC000), 0);
}

TEST(Memory, EchoRam) {
	gg::Memory mem;
	mem.write(0xC000, 67);
	EXPECT_EQ(mem.read(0xE000), 67);
}

TEST(Memory, ProhibitedMemory) {
	gg::Memory mem;
	EXPECT_EQ(mem.read(0xFE9F), 0);
	EXPECT_EQ(mem.read(0xFEA0), 0xAA);
	EXPECT_EQ(mem.read(0xFEB8), 0xBB);
	EXPECT_EQ(mem.read(0xFEFF), 0xFF);
	EXPECT_EQ(mem.read(0xFF00), 0);
}
