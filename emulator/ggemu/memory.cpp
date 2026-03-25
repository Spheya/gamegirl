#include "memory.hpp"

namespace gg {

	Memory::Memory() : m_memory(new uint8_t[0x10000]) {
		memset(m_memory, 0, 0x10000);
		for(auto it = UnusedMemory.begin; it != UnusedMemory.end; ++it) {
			auto value = uint8_t(it & 0xF0);
			m_memory[it] = value | (value >> 4);
		}
	}

	Memory::Memory(Memory&& other) noexcept : m_memory(other.m_memory) {
		other.m_memory = nullptr;
	}

	Memory& Memory::operator=(Memory&& other) noexcept {
		delete[] m_memory;
		m_memory = other.m_memory;
		other.m_memory = nullptr;
		return *this;
	}

	Memory::~Memory() {
		delete[] m_memory;
	}

} // namespace gg
