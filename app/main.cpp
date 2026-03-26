#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <string>
#include <vector>

#include <ggemu/emulator.hpp>
#include <ggemu/logger.hpp>

#include "tracer.hpp"

namespace {
	struct Options {
		bool trace;
		size_t traceNum;
	};
} // namespace

static void messageCallback(std::string_view message, gg::LogLevel level) {
	switch(level) {
	case gg::LogLevel::Info: std::print("\033[1;32mInfo > \033[0m"); break;
	case gg::LogLevel::Warn: std::print("\033[1;33mWarn > \033[0m"); break;
	case gg::LogLevel::Error: std::print("\033[1;31mError > \033[0m"); break;
	default: std::print("> "); break;
	}
	std::println("{}", message);
}

static void printUsage() {
	std::println("Usage: gamegirl <path-to-rom-file> [options]");
	std::println("Available Options:");
	std::println("  --trace [numSteps] : produces a file containing the state of the gameboy at every operation");
}

int main(int argc, const char* args[]) {
	if(argc < 2) {
		printUsage();
		return -1;
	}

	const char* filePath = args[1];

	// Load options
	Options options = {};
	for(int i = 2; i < argc; ++i) {
		if(std::strcmp(args[i], "--trace") == 0) {
			options.trace = true;
			if(argc > i + 1) {
				size_t pos;
				std::string param = args[i];
				options.traceNum = std::stoi(param, &pos);
				if(pos == param.size()) {
					++i;
				} else {
					std::println("Invalid option provided: {}", args[i]);
					printUsage();
					return -1;
				}
			}
		} else {
			std::println("Invalid option provided: {}", args[i]);
			printUsage();
			return -1;
		}
	}

	// Setup emulator
	gg::Logger::messageCallback = messageCallback;
	gg::Emulator emulator;

	// Load rom file
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

	// Setup logger
	gg::Logger::messageCallback = [&emulator](std::string_view message, gg::LogLevel level) {
		std::print("[${:04X}] ", emulator.registers.PC);
		messageCallback(message, level);
	};

	// Setup tracer
	if(options.trace) emulator.traceCallback = Tracer(std::make_shared<std::ostream>(std::cout.rdbuf()), options.traceNum);

	emulator.step();

	gg::Logger::log("hi!");
	gg::Logger::info("hi!");
	gg::Logger::warn("hi!");
	gg::Logger::error("hi!");
}
