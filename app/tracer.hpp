#pragma once

#include <cassert>
#include <format>
#include <ostream>

#include <ggemu/emulator.hpp>

#include "ggemu/register.hpp"

class Tracer {
public:
	Tracer(std::shared_ptr<std::basic_ostream<char>> stream, size_t numTraces) : m_output(std::move(stream)), m_tracesLeft(numTraces) {
		assert(m_output);
	}

	void operator()(gg::Emulator& emulator) {
		*m_output << std::format(
		    "A:{:02X} F:{:02X} B:{:02X} C:{:02X} D:{:02X} E:{:02X} H:{:02X} L:{:02X} SP:{:04X} PC:{:04X} PCMEM:{:02X},{:02X},{:02X},{:02X}\n",
		    emulator.registers.read8(gg::Register::A),
		    emulator.registers.read8(gg::Register::F),
		    emulator.registers.read8(gg::Register::B),
		    emulator.registers.read8(gg::Register::C),
		    emulator.registers.read8(gg::Register::D),
		    emulator.registers.read8(gg::Register::E),
		    emulator.registers.read8(gg::Register::H),
		    emulator.registers.read8(gg::Register::L),
		    emulator.registers.SP,
		    emulator.registers.PC,
		    emulator.memory.read(emulator.registers.PC),
		    emulator.memory.read(emulator.registers.PC + 1),
		    emulator.memory.read(emulator.registers.PC + 2),
		    emulator.memory.read(emulator.registers.PC + 3)
		);
		if(--m_tracesLeft == 0) {
			emulator.traceCallback = nullptr;
			m_output.reset();
		}
	}

private:
	std::shared_ptr<std::basic_ostream<char>> m_output;
	size_t m_tracesLeft;
};
