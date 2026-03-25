#pragma once

#include <cassert>
#include <cstring>

#include "logger.hpp"
#include "types.hpp"

namespace gg {

	class Memory {
	public:
		constexpr static MemoryRange Rom = { .begin = 0x0000, .end = 0x8000 };
		constexpr static MemoryRange VideoRam = { .begin = 0x8000, .end = 0xA000 };
		constexpr static MemoryRange ExternalRam = { .begin = 0xA000, .end = 0xC000 };
		constexpr static MemoryRange WorkRam = { .begin = 0xC000, .end = 0xE000 };
		constexpr static MemoryRange EchoRam = { .begin = 0xE000, .end = 0xFE00 };
		constexpr static MemoryRange ObjectAttributeMemory = { .begin = 0xFE00, .end = 0xFEA0 };
		constexpr static MemoryRange UnusedMemory = { .begin = 0xFEA0, .end = 0xFF00 };
		constexpr static MemoryRange IORegisters = { .begin = 0xFF00, .end = 0xFF80 };
		constexpr static MemoryRange HighRam = { .begin = 0xFF80, .end = 0xFFFF };
		constexpr static ptr InterruptRegister = 0xFFFF;

	public:
		Memory();
		Memory(Memory&) = delete;
		Memory& operator=(Memory&) = delete;
		Memory(Memory&& other) noexcept;
		Memory& operator=(Memory&& other) noexcept;
		~Memory();

		[[nodiscard]] uint8_t read(ptr address) const;
		void write(ptr address, uint8_t value);

		void copy(void* dst, MemoryRange range) const;
		void copy(void* dst, ptr src, size size) const;
		void copy(ptr dst, const void* src, size size);

	private:
		void destroy();

	private:
		uint8_t* m_memory;
	};

	inline uint8_t Memory::read(ptr address) const {
		if(EchoRam.contains(address)) {
			Logger::warn("Reading from echo RAM ({:#06x})", address);
			address -= EchoRam.begin - WorkRam.begin;
		} else if(UnusedMemory.contains(address)) {
			Logger::warn("Reading from prohibited memory ({:#06x})", address);
		}
		return m_memory[address];
	}

	inline void Memory::write(ptr address, uint8_t value) {
		if(EchoRam.contains(address)) {
			Logger::warn("Writing to echo RAM ({:#06x})", address);
			address -= EchoRam.begin - WorkRam.begin;
		} else if(UnusedMemory.contains(address)) {
			Logger::warn("Writing to prohibited memory ({:#06x})", address);
			return;
		} else if(Rom.contains(address)) {
			Logger::warn("Writing to read-only memory ({:#06x})", address);
		}
		m_memory[address] = value;
	}

	inline void Memory::copy(void* dst, MemoryRange range) const {
		copy(dst, range.begin, range.size());
	}

	inline void Memory::copy(void* dst, ptr src, size size) const {
		assert(!EchoRam.overlaps(MemoryRange{ .begin = src, .end = uint16_t(src + size) }));
		assert(!UnusedMemory.overlaps(MemoryRange{ .begin = src, .end = uint16_t(src + size) }));
		memcpy(dst, m_memory + src, size);
	}

	inline void Memory::copy(ptr dst, const void* src, size size) {
		assert(!EchoRam.overlaps(MemoryRange{ .begin = dst, .end = uint16_t(dst + size) }));
		assert(!UnusedMemory.overlaps(MemoryRange{ .begin = dst, .end = uint16_t(dst + size) }));
		memcpy(m_memory + dst, src, size);
	}

} // namespace gg
