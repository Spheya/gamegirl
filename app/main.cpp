#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <vector>

#include <ggemu/emulator.hpp>
#include <ggemu/logger.hpp>

struct Options {
	bool log;
};

static void messageCallback(std::string_view message, gg::LogLevel level) {
	switch(level) {
	case gg::LogLevel::Info: std::print("\033[1;32mInfo > \033[0m"); break;
	case gg::LogLevel::Warn: std::print("\033[1;33mWarn > \033[0m"); break;
	case gg::LogLevel::Error: std::print("\033[1;31mError > \033[0m"); break;
	default: std::print("> "); break;
	}
	std::println("{}", message);
}

int main(int argc, const char* args[]) {
	if(argc < 2) {
		std::println("Usage: gamegirl <path-to-rom-file> [options]");
		return -1;
	}

	const char* filePath = args[1];

	gg::Logger::messageCallback = messageCallback;
	gg::Emulator emulator;

	{
		// Todo: not hardcode the rom file
		std::ifstream romFile(filePath, std::ios::binary | std::ios::ate);
		if(!romFile.is_open()) {
			std::println("Could not read ROM file: could not open file");
			return -2;
		}
		size_t fileSize = size_t(romFile.tellg());
		if(fileSize > 0xFFFF) {
			std::println("Could not read ROM file: file too big");
			return -3;
		}
		auto romSize = gg::size(fileSize);
		std::vector<char> buffer(romSize);
		romFile.seekg(0);
		if(!romFile.read(buffer.data(), romSize)) {
			std::println("Could not read ROM file: error reading file");
			return -4;
		}

		emulator.loadRom(buffer.data(), romSize);
	}

	gg::Logger::messageCallback = [&emulator](std::string_view message, gg::LogLevel level) {
		std::print("[${:04x}] ", emulator.registers.PC);
		messageCallback(message, level);
	};

	gg::Logger::log("hi!");
	gg::Logger::info("hi!");
	gg::Logger::warn("hi!");
	gg::Logger::error("hi!");
}
