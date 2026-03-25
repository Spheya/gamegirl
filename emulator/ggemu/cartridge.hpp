#pragma once

#include <cstdint>
#include <string_view>

namespace gg {

	enum class CartridgeType : uint8_t {
		RomOnly = 0x00,
		Mbc1 = 0x01,
		Mbc1Ram = 0x02,
		Mbc1RamBattery = 0x03,
		Mbc2 = 0x05,
		Mbc2Battery = 0x06,
		RomRam = 0x08,
		RomRamBattery = 0x09,
		Mmm01 = 0x0B,
		Mmm01Ram = 0x0C,
		Mmm01RamBattery = 0x0D,
		Mbc3TimerBattery = 0x0F,
		Mbc3TimerRamBattery = 0x10,
		Mbc3 = 0x11,
		Mbc3Ram = 0x12,
		Mbc3RamBattery = 0x13,
		Mbc5 = 0x19,
		Mbc5Ram = 0x1A,
		Mbc5RamBattery = 0x1B,
		Mbc5Rumble = 0x1C,
		Mbc5RumbleRam = 0x1D,
		Mbc5RumbleRamBattery = 0x1E,
		Mbc6 = 0x20,
		Mbc7SensorRumbleRamBattery = 0x22,
		PocketCamera = 0xFC,
		BandaiTama5 = 0xFD,
		HuC3 = 0xFE,
		HuC1RamBattery = 0xFF
	};

	enum class RomSize : uint8_t {
		_32KiB = 0x00,
		_64KiB = 0x01,
		_128KiB = 0x02,
		_256KiB = 0x03,
		_512KiB = 0x04,
		_1MiB = 0x05,
		_2MiB = 0x06,
		_4MiB = 0x07,
		_8MiB = 0x08,
		_1_1MiB = 0x52,
		_1_2MiB = 0x53,
		_1_5MiB = 0x54
	};

	enum class RamSize : uint8_t { _0 = 0, _2KiB = 0x1, _8KiB = 0x2, _32KiB = 0x3, _128KiB = 0x4, _64KiB = 0x5 };

	enum class DestinationCode : uint8_t { Japan = 0x0, OverseasOnly = 0x1 };

	[[nodiscard]] std::string_view to_string(CartridgeType cardridgeType);
	[[nodiscard]] std::string_view to_string(RomSize romSize);
	[[nodiscard]] std::string_view to_string(RamSize ramSize);
	[[nodiscard]] std::string_view to_string(DestinationCode destinationCode);

	struct CartridgeHeader {
		uint8_t logo[48];
		char title[16];
		uint16_t newLicenseeCode;
		uint8_t sgbFlag;
		CartridgeType cartridgeType;
		RomSize romSize;
		RamSize ramSize;
		DestinationCode destinationCode;
		uint8_t oldLicenseeCode;
		uint8_t maskRomVersionNumber;
		uint8_t headerChecksum;
		uint16_t globalChecksum;
	};

	class CartridgeInfo {
	public:
		CartridgeInfo() : header{} {}
		explicit CartridgeInfo(CartridgeHeader header) : header(header) {}

		void logInfo() const;

		[[nodiscard]] std::string_view getTitle() const;
		[[nodiscard]] std::string_view getManufacturer() const;
		[[nodiscard]] std::string_view getLicensee() const;

	public:
		CartridgeHeader header;
	};

} // namespace gg
