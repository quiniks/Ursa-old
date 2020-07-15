#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Ursa {

	class URSA_API Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
			return s_CoreLogger;
		}
		static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return s_ClientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

//Core log macros
#define URSA_CORE_TRACE(...)	::Ursa::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define URSA_CORE_INFO(...)		::Ursa::Log::GetCoreLogger()->info(__VA_ARGS__)
#define URSA_CORE_WARN(...)		::Ursa::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define URSA_CORE_ERROR(...)	::Ursa::Log::GetCoreLogger()->error(__VA_ARGS__)
#define URSA_CORE_CRITICAL(...)	::Ursa::Log::GetCoreLogger()->critical(__VA_ARGS__)
//Client log macros
#define URSA_TRACE(...)	::Ursa::Log::GetClientLogger()->trace(__VA_ARGS__)
#define URSA_INFO(...)	::Ursa::Log::GetClientLogger()->info(__VA_ARGS__)
#define URSA_WARN(...)	::Ursa::Log::GetClientLogger()->warn(__VA_ARGS__)
#define URSA_ERROR(...)	::Ursa::Log::GetClientLogger()->error(__VA_ARGS__)
#define URSA_CRITICAL(...)	::Ursa::Log::GetClientLogger()->critical(__VA_ARGS__)