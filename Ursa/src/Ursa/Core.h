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