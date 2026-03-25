#pragma once

#include <cassert>
#include <cstdint>

namespace gg {

	enum class Register {
		A,
		F,
		B,
		C,
		D,
		E,
		H,
		L,
		AF,
		BC,
		DE,
		HL,
		SP,
		PC,
	};

	struct RegisterContext {
		uint16_t AF;
		uint16_t BC;
		uint16_t DE;
		uint16_t HL;
		uint16_t SP;
		uint16_t PC;

		[[nodiscard]] constexpr uint8_t read8(Register r) const {
			assert(int(r) < int(Register::AF));
			uint16_t tmp = (&AF)[int(r) >> 1];
			return (int(r) & 0x1) ? uint8_t(tmp) : uint8_t(tmp >> 8);
		}

		constexpr void write8(Register r, uint8_t value) {
			assert(int(r) < int(Register::AF));
			uint16_t& target = (&AF)[int(r) >> 1];
			if(int(r) & 0x1) {
				target = (target & 0xFF00) | uint16_t(value);
			} else {
				target = (target & 0x00FF) | (uint16_t(value) << 8);
			}
		}

		[[nodiscard]] constexpr uint16_t read16(Register r) const {
			assert(int(r) >= int(Register::AF));
			return (&AF)[int(r) - int(Register::AF)];
		}

		constexpr void write16(Register r, uint16_t value) {
			assert(int(r) >= int(Register::AF));
			(&AF)[int(r) - int(Register::AF)] = value;
		}

		[[nodiscard]] constexpr uint16_t read(Register r) const {
			if(int(r) < int(Register::AF)) return uint16_t(read8(r));
			return read16(r);
		}

		constexpr void write(Register r, uint16_t value) {
			if(int(r) < int(Register::AF))
				write8(r, uint8_t(value));
			else
				write16(r, value);
		}
	};

} // namespace gg
