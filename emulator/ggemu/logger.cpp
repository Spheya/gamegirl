#include "logger.hpp"

namespace gg {

    std::function<void(std::string_view, LogLevel)> Logger::messageCallback;

}