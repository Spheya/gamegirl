#include "emulator.hpp"

#include <bit>

#include "cartridge.hpp"
#include "logger.hpp"
#include "memory.hpp"

namespace gg {

	Emulator::Emulator() : registers{}, m_cartridgeHeader{} {}

	bool Emulator::loadRom(const void* romData, size size) {
		if(size > Memory::Rom.size()) {
			Logger::error("ROM is too big! Maximum size is: {}", Memory::Rom.size());
			return false;
		}
		if(size < 0x104 + sizeof(CartridgeHeader)) {
			Logger::error("ROM is too small! Minimum size is: {}", 0x104 + sizeof(CartridgeHeader));
			return false;
		}

		// Load header
		memcpy(&m_cartridgeHeader, reinterpret_cast<const char*>(romData) + 0x104, sizeof(CartridgeHeader));
		if constexpr(std::endian::native == std::endian::big) {
			m_cartridgeHeader.newLicenseeCode = std::byteswap(m_cartridgeHeader.newLicenseeCode);
		} else {
			m_cartridgeHeader.globalChecksum = std::byteswap(m_cartridgeHeader.globalChecksum);
		}

		CartridgeInfo info(m_cartridgeHeader);
		info.logInfo();

		bool valid = true;

		// Checksum validation
		uint8_t checksum = 0;
		for(uint16_t address = 0x0134; address <= 0x014C; address++) checksum = checksum - reinterpret_cast<const uint8_t*>(romData)[address] - 1;

		if(checksum != m_cartridgeHeader.headerChecksum) {
			valid = false;
			Logger::error("Invalid header checksum detected (found: {:#04x}, expected: {:#04x})", m_cartridgeHeader.headerChecksum, checksum);
		}

		// Global checksum validation
		uint16_t globalChecksum = -((m_cartridgeHeader.globalChecksum & 0xFF) + (m_cartridgeHeader.globalChecksum >> 8));
		for(uint16_t i = 0; i != size; ++i) globalChecksum += reinterpret_cast<const uint8_t*>(romData)[i];

		if(globalChecksum != m_cartridgeHeader.globalChecksum)
			Logger::warn("Invalid global checksum detected (found: {:#06x}, expected: {:#06x})", m_cartridgeHeader.globalChecksum, globalChecksum);

		// Logo validation
		constexpr uint8_t expectedLogo[48] = { 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
			                                   0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
			                                   0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E };

		if(memcmp(expectedLogo, m_cartridgeHeader.logo, sizeof(expectedLogo)) != 0) Logger::warn("Invalid logo detected");

		// Load ROM if everything is valid
		if(valid) {
			memory.copy(Memory::Rom.begin, romData, size);
			registers.PC = 0x100;
			return true;
		}
		return false;
	}

} // namespace gg
