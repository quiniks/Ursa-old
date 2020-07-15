#pragma once

#ifdef URSA_PLATFORM_WINDOWS
	#ifdef URSA_BUILD_DLL
		#define URSA_API __declspec(dllexport)
	#else
		#define URSA_API __declspec(dllimport)
	#endif
#else
	#error Ursa only supports Windows
#endif

#ifdef URSA_ENABLE_ASSERTS
	#define URSA_ASSERT(x, ...) {if(!(x)) {URSA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define URSA_CORE_ASSERT(x, ...) {if(!(x)) {URSA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define URSA_ASSERT(x, ...)
	#define URSA_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)