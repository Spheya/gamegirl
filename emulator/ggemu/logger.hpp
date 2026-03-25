#pragma once

#include <format>
#include <functional>
#include <string_view>

namespace gg {

	enum class LogLevel { Trace, Info, Warn, Error };

	class Logger {
	public:
#ifdef NDEBUG
		template<typename... T>
		static void log([[maybe_unused]] std::format_string<T...> fmt, [[maybe_unused]] T&&... args) {}
#else
		template<typename... T>
		static void log(std::format_string<T...> fmt, T&&... args) {
			if(messageCallback) messageCallback(std::format(fmt, std::forward<T&&>(args)...), LogLevel::Trace);
		}
#endif

		template<typename... T>
		static void info(std::format_string<T...> fmt, T&&... args) {
			if(messageCallback) messageCallback(std::format(fmt, std::forward<T&&>(args)...), LogLevel::Info);
		}

		template<typename... T>
		static void warn(std::format_string<T...> fmt, T&&... args) {
			if(messageCallback) messageCallback(std::format(fmt, std::forward<T&&>(args)...), LogLevel::Warn);
		}

		template<typename... T>
		static void error(std::format_string<T...> fmt, T&&... args) {
			if(messageCallback) messageCallback(std::format(fmt, std::forward<T&&>(args)...), LogLevel::Error);
		}

	public:
		static std::function<void(std::string_view, LogLevel)> messageCallback;
	};

} // namespace gg
