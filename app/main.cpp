#include <iostream>

#include <ggemu/logger.hpp>

static void messageCallback(std::string_view message, gg::LogLevel level) {
	switch(level) {
	case gg::LogLevel::Info: std::cout << "\033[1;32mInfo > \033[0m"; break;
	case gg::LogLevel::Warn: std::cout << "\033[1;33mWarn > \033[0m"; break;
	case gg::LogLevel::Error: std::cout << "\033[1;31mError > \033[0m"; break;
	default: std::cout << "> "; break;
	}
	std::cout << message << "\n";
}

int main() {
	gg::Logger::messageCallback = messageCallback;
	gg::Logger::log("hi!");
	gg::Logger::info("hi!");
	gg::Logger::warn("hi!");
	gg::Logger::error("hi!");
}
