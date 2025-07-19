#pragma once
#include <memory>
#include <spdlog/spdlog.h>

namespace Cube {
    class Log {
    public:
        static void init();
        inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
        inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> coreLogger;
        static std::shared_ptr<spdlog::logger> clientLogger;
    };
}  // namespace Cube

// Cube内部日志

#ifdef CB_DEBUG
    #define CB_CORE_TRACE(...) ::Cube::Log::getCoreLogger()->trace(__VA_ARGS__)
    #define CB_CORE_INFO(...) ::Cube::Log::getCoreLogger()->info(__VA_ARGS__)
    #define CB_CORE_WARN(...) ::Cube::Log::getCoreLogger()->warn(__VA_ARGS__)
    #define CB_CORE_ERROR(...) ::Cube::Log::getCoreLogger()->error(__VA_ARGS__)
    #define CB_CORE_CRITICAL(...) ::Cube::Log::getCoreLogger()->critical(__VA_ARGS__)

    // 客户端日志
    #define CB_TRACE(...) ::Cube::Log::getClientLogger()->trace(__VA_ARGS__)
    #define CB_INFO(...) ::Cube::Log::getClientLogger()->info(__VA_ARGS__)
    #define CB_WARN(...) ::Cube::Log::getClientLogger()->warn(__VA_ARGS__)
    #define CB_ERROR(...) ::Cube::Log::getClientLogger()->error(__VA_ARGS__)
    #define CB_CRITICAL(...) ::Cube::Log::getClientLogger()->critical(__VA_ARGS__)

    // 断言
    #define CB_ASSERT(...) assert(__VA_ARGS__)
#else
    #define CB_CORE_TRACE(...) 
    #define CB_CORE_INFO(...)
    #define CB_CORE_WARN(...)
    #define CB_CORE_ERROR(...)
    #define CB_CORE_CRITICAL(...)

    // 客户端日志
    #define CB_TRACE(...)
    #define CB_INFO(...)
    #define CB_WARN(...)
    #define CB_ERROR(...)
    #define CB_CRITICAL(...)

    // 断言
    #define CB_ASSERT(...)
#endif