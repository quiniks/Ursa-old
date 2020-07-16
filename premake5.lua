workspace "Ursa"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (sln directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Ursa/vendor/GLFW/include"
IncludeDir["Glad"] = "Ursa/vendor/Glad/include"
IncludeDir["ImGui"] = "Ursa/vendor/imgui"

include "Ursa/vendor/GLFW"
include "Ursa/vendor/Glad"
include "Ursa/vendor/ImGui"

project "Ursa"
	location "Ursa"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ursapch.h"
	pchsource "Ursa/src/ursapch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"URSA_PLATFORM_WINDOWS",
			"URSA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

		filter "configurations:Debug"
			defines "URSA_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "URSA_RELEASE"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "URSA_DIST"
			buildoptions "/MD"
			optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Ursa/vendor/spdlog/include",
		"Ursa/src"
	}

	links {
		"Ursa"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"URSA_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "URSA_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "URSA_RELEASE"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "URSA_DIST"
			buildoptions "/MD"
			optimize "On"