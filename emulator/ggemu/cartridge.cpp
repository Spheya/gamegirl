#include "cartridge.hpp"
#include "logger.hpp"

namespace gg {

	std::string_view to_string(CartridgeType cardridgeType) {
		switch(cardridgeType) {
		case CartridgeType::RomOnly: return "ROM Only";
		case CartridgeType::Mbc1: return "MBC1";
		case CartridgeType::Mbc1Ram: return "MBC1 + RAM";
		case CartridgeType::Mbc1RamBattery: return "MBC1 + RAM + Battery";
		case CartridgeType::Mbc2: return "MBC2";
		case CartridgeType::Mbc2Battery: return "MBC2 + Battery";
		case CartridgeType::RomRam: return "ROM + RAM";
		case CartridgeType::RomRamBattery: return "ROM + RAM + Battery";
		case CartridgeType::Mmm01: return "MMM01";
		case CartridgeType::Mmm01Ram: return "MMM01 + RAM";
		case CartridgeType::Mmm01RamBattery: return "MMM01 + RAM + Battery";
		case CartridgeType::Mbc3TimerBattery: return "MBC3 + Timer + Battery";
		case CartridgeType::Mbc3TimerRamBattery: return "MBC3 + Timer + RAM + Battery";
		case CartridgeType::Mbc3: return "MBC3";
		case CartridgeType::Mbc3Ram: return "MBC3 + RAM";
		case CartridgeType::Mbc3RamBattery: return "MBC3 + RAM + Battery";
		case CartridgeType::Mbc5: return "MBC5";
		case CartridgeType::Mbc5Ram: return "MBC5 + RAM";
		case CartridgeType::Mbc5RamBattery: return "MBC5 + RAM + Battery";
		case CartridgeType::Mbc5Rumble: return "MBC5 + Rumble";
		case CartridgeType::Mbc5RumbleRam: return "MBC5 + Rumble + RAM";
		case CartridgeType::Mbc5RumbleRamBattery: return "MBC5 + Rumble + RAM + Battery";
		case CartridgeType::Mbc6: return "MBC6";
		case CartridgeType::Mbc7SensorRumbleRamBattery: return "MBC7 + Sensor + Rumble + RAM + Battery";
		case CartridgeType::PocketCamera: return "Pocket Camera";
		case CartridgeType::BandaiTama5: return "Bandai Tama5";
		case CartridgeType::HuC3: return "HuC3";
		case CartridgeType::HuC1RamBattery: return "HuC1 + RAM + Battery";
		default: return "???";
		}
	}

	std::string_view to_string(RomSize romSize) {
		switch(romSize) {
		case RomSize::_32KiB: return "32 KiB";
		case RomSize::_64KiB: return "64 KiB";
		case RomSize::_128KiB: return "128 KiB";
		case RomSize::_256KiB: return "256 KiB";
		case RomSize::_512KiB: return "512 KiB";
		case RomSize::_1MiB: return "1 MiB";
		case RomSize::_2MiB: return "2 MiB";
		case RomSize::_4MiB: return "4 MiB";
		case RomSize::_8MiB: return "8 MiB";
		case RomSize::_1_1MiB: return "1.1 MiB";
		case RomSize::_1_2MiB: return "1.2 MiB";
		case RomSize::_1_5MiB: return "1.5 MiB";
		default: return "???";
		}
	}

	std::string_view to_string(RamSize ramSize) {
		switch(ramSize) {
		case RamSize::_0: return "No RAM";
		case RamSize::_2KiB: return "2 KiB";
		case RamSize::_8KiB: return "8 KiB";
		case RamSize::_32KiB: return "32 KiB";
		case RamSize::_128KiB: return "128 KiB";
		case RamSize::_64KiB: return "64 KiB";
		default: return "???";
		}
	}

	std::string_view to_string(DestinationCode destinationCode) {
		switch(destinationCode) {
		case DestinationCode::Japan: return "Japan (and possibly overseas)";
		case DestinationCode::OverseasOnly: return "Overseas only";
		default: return "???";
		}
	}

	void CartridgeInfo::logInfo() const {
		Logger::info("Title: {}", getTitle());

		std::string_view licensee = getLicensee();
		if(licensee[0] == '?') {
			Logger::error("Licensee: {}", licensee);
		} else {
			Logger::info("Licensee: {}", licensee);
		}

		std::string_view cartridgeType = to_string(header.cartridgeType);
		if(header.cartridgeType == CartridgeType::RomRam || header.cartridgeType == CartridgeType::RomRamBattery) {
			Logger::warn("Cartridge Type: {} (Unused)", cartridgeType);
		} else {
			if(cartridgeType[0] == '?') {
				Logger::error("Cartridge Type: {}", cartridgeType);
			} else {
				Logger::info("Cartridge Type: {}", cartridgeType);
			}
		}

		std::string_view romSize = to_string(header.romSize);
		if(header.romSize == RomSize::_1_1MiB || header.romSize == RomSize::_1_2MiB || header.romSize == RomSize::_1_5MiB) {
			Logger::warn("ROM Size: {} (Unused)", romSize);
		} else {
			if(romSize[0] == '?') {
				Logger::error("ROM Size: {}", romSize);
			} else {
				Logger::info("ROM Size: {}", romSize);
			}
		}

		std::string_view ramSize = to_string(header.ramSize);
		if(header.ramSize == RamSize::_2KiB) {
			Logger::warn("RAM Size: {} (Unused)", ramSize);
		} else {
			if(ramSize[0] == '?') {
				Logger::error("RAM Size: {}", ramSize);
			} else {
				Logger::info("RAM Size: {}", ramSize);
			}
		}

		Logger::info("ROM Version Number: {}", header.maskRomVersionNumber);

		std::string_view destinationCode = to_string(header.destinationCode);
		if(destinationCode[0] == '?') {
			Logger::error("Destination Code: {}\n", destinationCode);
		} else {
			Logger::info("Destination Code: {}\n", destinationCode);
		}
	}

	std::string_view CartridgeInfo::getTitle() const {
		for(int size = 0; size < 16; ++size)
			if(header.title[size] == '\0') return std::string_view(header.title, size);
		return std::string_view(header.title, 16);
	}

	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
	std::string_view CartridgeInfo::getManufacturer() const {
		return "manufacturer parsing not implemented";
	}

	std::string_view CartridgeInfo::getLicensee() const {
		if(header.oldLicenseeCode == 0x33) {
			switch(header.newLicenseeCode) {
			case '00': return "None";
			case '01': return "Nintendo Research & Development 1";
			case '08': return "Capcom";
			case '13': return "EA(Electronic Arts)";
			case '18': return "Hudson Soft";
			case '19': return "B - AI";
			case '20': return "KSS";
			case '22': return "Planning Office WADA";
			case '24': return "PCM Complete";
			case '25': return "San - X";
			case '28': return "Kemco";
			case '29': return "SETA Corporation";
			case '30': return "Viacom";
			case '31': return "Nintendo";
			case '32': return "Bandai";
			case '33': return "Ocean Software / Acclaim Entertainment";
			case '34': return "Konami";
			case '35': return "HectorSoft";
			case '37': return "Taito";
			case '38': return "Hudson Soft";
			case '39': return "Banpresto";
			case '41': return "Ubi Soft1";
			case '42': return "Atlus";
			case '44': return "Malibu Interactive";
			case '46': return "Angel";
			case '47': return "Bullet - Proof Software2";
			case '49': return "Irem";
			case '50': return "Absolute";
			case '51': return "Acclaim Entertainment";
			case '52': return "Activision";
			case '53': return "Sammy USA Corporation";
			case '54': return "Konami";
			case '55': return "Hi Tech Expressions";
			case '56': return "LJN";
			case '57': return "Matchbox";
			case '58': return "Mattel";
			case '59': return "Milton Bradley Company";
			case '60': return "Titus Interactive";
			case '61': return "Virgin Games Ltd.3";
			case '64': return "Lucasfilm Games4";
			case '67': return "Ocean Software";
			case '69': return "EA(Electronic Arts)";
			case '70': return "Infogrames5";
			case '71': return "Interplay Entertainment";
			case '72': return "Broderbund";
			case '73': return "Sculptured Software6";
			case '75': return "The Sales Curve Limited7";
			case '78': return "THQ";
			case '79': return "Accolade8";
			case '80': return "Misawa Entertainment";
			case '83': return "LOZC G.";
			case '86': return "Tokuma Shoten";
			case '87': return "Tsukuda Original";
			case '91': return "Chunsoft Co.9";
			case '92': return "Video System";
			case '93': return "Ocean Software / Acclaim Entertainment";
			case '95': return "Varie";
			case '96': return "Yonezawa10 / S\'Pal";
			case '97': return "Kaneko";
			case '99': return "Pack - In - Video";
			case '9H': return "Bottom Up";
			case 'A4': return "Konami(Yu - Gi - Oh!)";
			case 'BL': return "MTO";
			case 'DK': return "Kodansha";
			default: return "???";
			}
		} else {
			switch(header.oldLicenseeCode) {
			case 0x00: return "None";
			case 0x01: return "Nintendo";
			case 0x08: return "Capcom";
			case 0x09: return "HOT - B";
			case 0x0A: return "Jaleco";
			case 0x0B: return "Coconuts Japan";
			case 0x0C: return "Elite Systems";
			case 0x13: return "EA(Electronic Arts)";
			case 0x18: return "Hudson Soft";
			case 0x19: return "ITC Entertainment";
			case 0x1A: return "Yanoman";
			case 0x1D: return "Japan Clary";
			case 0x1F: return "Virgin Games Ltd.3";
			case 0x24: return "PCM Complete";
			case 0x25: return "San - X";
			case 0x28: return "Kemco";
			case 0x29: return "SETA Corporation";
			case 0x30: return "Infogrames5";
			case 0x31: return "Nintendo";
			case 0x32: return "Bandai";
			case 0x34: return "Konami";
			case 0x35: return "HectorSoft";
			case 0x38: return "Capcom";
			case 0x39: return "Banpresto";
			case 0x3C: return "Entertainment Interactive(stub)";
			case 0x3E: return "Gremlin";
			case 0x41: return "Ubi Soft1";
			case 0x42: return "Atlus";
			case 0x44: return "Malibu Interactive";
			case 0x46: return "Angel";
			case 0x47: return "Spectrum HoloByte";
			case 0x49: return "Irem";
			case 0x4A: return "Virgin Games Ltd.3";
			case 0x4D: return "Malibu Interactive";
			case 0x4F: return "U.S.Gold";
			case 0x50: return "Absolute";
			case 0x51: return "Acclaim Entertainment";
			case 0x52: return "Activision";
			case 0x53: return "Sammy USA Corporation";
			case 0x54: return "GameTek";
			case 0x55: return "Park Place15";
			case 0x56: return "LJN";
			case 0x57: return "Matchbox";
			case 0x59: return "Milton Bradley Company";
			case 0x5A: return "Mindscape";
			case 0x5B: return "Romstar";
			case 0x5C: return "Naxat Soft16";
			case 0x5D: return "Tradewest";
			case 0x60: return "Titus Interactive";
			case 0x61: return "Virgin Games Ltd.3";
			case 0x67: return "Ocean Software";
			case 0x69: return "EA(Electronic Arts)";
			case 0x6E: return "Elite Systems";
			case 0x6F: return "Electro Brain";
			case 0x70: return "Infogrames5";
			case 0x71: return "Interplay Entertainment";
			case 0x72: return "Broderbund";
			case 0x73: return "Sculptured Software6";
			case 0x75: return "The Sales Curve Limited7";
			case 0x78: return "THQ";
			case 0x79: return "Accolade8";
			case 0x7A: return "Triffix Entertainment";
			case 0x7C: return "MicroProse";
			case 0x7F: return "Kemco";
			case 0x80: return "Misawa Entertainment";
			case 0x83: return "LOZC G.";
			case 0x86: return "Tokuma Shoten";
			case 0x8B: return "Bullet - Proof Software2";
			case 0x8C: return "Vic Tokai Corp.17";
			case 0x8E: return "Ape Inc.18";
			case 0x8F: return "I\'Max19";
			case 0x91: return "Chunsoft Co.9";
			case 0x92: return "Video System";
			case 0x93: return "Tsubaraya Productions";
			case 0x95: return "Varie";
			case 0x96: return "Yonezawa10 / S\'Pal";
			case 0x97: return "Kemco";
			case 0x99: return "Arc";
			case 0x9A: return "Nihon Bussan";
			case 0x9B: return "Tecmo";
			case 0x9C: return "Imagineer";
			case 0x9D: return "Banpresto";
			case 0x9F: return "Nova";
			case 0xA1: return "Hori Electric";
			case 0xA2: return "Bandai";
			case 0xA4: return "Konami";
			case 0xA6: return "Kawada";
			case 0xA7: return "Takara";
			case 0xA9: return "Technos Japan";
			case 0xAA: return "Broderbund";
			case 0xAC: return "Toei Animation";
			case 0xAD: return "Toho";
			case 0xAF: return "Namco";
			case 0xB0: return "Acclaim Entertainment";
			case 0xB1: return "ASCII Corporation or Nexsoft";
			case 0xB2: return "Bandai";
			case 0xB4: return "Square Enix";
			case 0xB6: return "HAL Laboratory";
			case 0xB7: return "SNK";
			case 0xB9: return "Pony Canyon";
			case 0xBA: return "Culture Brain";
			case 0xBB: return "Sunsoft";
			case 0xBD: return "Sony Imagesoft";
			case 0xBF: return "Sammy Corporation";
			case 0xC0: return "Taito";
			case 0xC2: return "Kemco";
			case 0xC3: return "Square";
			case 0xC4: return "Tokuma Shoten";
			case 0xC5: return "Data East";
			case 0xC6: return "Tonkin House";
			case 0xC8: return "Koei";
			case 0xC9: return "UFL";
			case 0xCA: return "Ultra Games";
			case 0xCB: return "VAP, Inc.";
			case 0xCC: return "Use Corporation";
			case 0xCD: return "Meldac";
			case 0xCE: return "Pony Canyon";
			case 0xCF: return "Angel";
			case 0xD0: return "Taito";
			case 0xD1: return "SOFEL(Software Engineering Lab)";
			case 0xD2: return "Quest";
			case 0xD3: return "Sigma Enterprises";
			case 0xD4: return "ASK Kodansha Co.";
			case 0xD6: return "Naxat Soft16";
			case 0xD7: return "Copya System";
			case 0xD9: return "Banpresto";
			case 0xDA: return "Tomy";
			case 0xDB: return "LJN";
			case 0xDD: return "Nippon Computer Systems";
			case 0xDE: return "Human Ent.";
			case 0xDF: return "Altron";
			case 0xE0: return "Jaleco";
			case 0xE1: return "Towa Chiki";
			case 0xE2: return "Yutaka # Needs more info";
			case 0xE3: return "Varie";
			case 0xE5: return "Epoch";
			case 0xE7: return "Athena";
			case 0xE8: return "Asmik Ace Entertainment";
			case 0xE9: return "Natsume";
			case 0xEA: return "King Records";
			case 0xEB: return "Atlus";
			case 0xEC: return "Epic / Sony Records";
			case 0xEE: return "IGS";
			case 0xF0: return "A Wave";
			case 0xF3: return "Extreme Entertainment";
			case 0xFF: return "LJN";
			default: return "???";
			}
		}
	}

} // namespace gg
