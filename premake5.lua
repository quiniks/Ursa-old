workspace "Ursa"
	architecture "x64"
	startproject "Sandbox"

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
IncludeDir["glm"] = "Ursa/vendor/glm"

group "Dependencies"
	include "Ursa/vendor/GLFW"
	include "Ursa/vendor/Glad"
	include "Ursa/vendor/ImGui"
group ""

project "Ursa"
	location "Ursa"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ursapch.h"
	pchsource "Ursa/src/ursapch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm//**.hpp",
		"%{prj.name}/vendor/glm//**.inl"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"URSA_PLATFORM_WINDOWS",
			"URSA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

		filter "configurations:Debug"
			defines "URSA_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "URSA_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "URSA_DIST"
			runtime "Release"
			optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Ursa/vendor/spdlog/include",
		"Ursa/src",
		"%{IncludeDir.glm}"
	}

	links {
		"Ursa"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"URSA_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "URSA_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "URSA_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "URSA_DIST"
			runtime "Release"
			optimize "On"