#pragma once

#include <functional>

#include "cartridge.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "types.hpp"

namespace gg {

	class Emulator {
	public:
		Emulator();

		bool loadRom(const void* romData, size size);

		void step();

		const CartridgeHeader& getCartridgeHeader() { return m_cartridgeHeader; }

	public:
		RegisterContext registers;
		Memory memory;

		std::function<void(Emulator& emulator)> traceCallback;

	private:
		CartridgeHeader m_cartridgeHeader;
	};

} // namespace gg
