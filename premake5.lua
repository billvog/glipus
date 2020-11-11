workspace "Glipus"
	configurations { "Debug32", "Release32", "Debug64", "Release64" }

	filter "configurations:*32"
		architecture "x86"
		defines { "GL_ARCH_86" }

	filter "configurations:*64"
		architecture "x86_64"
		defines { "GL_ARCH_64" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Glipus"
	location "Glipus"
	kind "WindowedApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir  ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/**.rc",
		"%{prj.name}/assets/**.ico"
	}

	includedirs {
		"$(WXWIN)/include/msvc",
		"$(WXWIN)/include",
		"%{wks.location}/Updater/src/"
	}

	links {
		"Updater"
	}

	filter "configurations:*64"
		libdirs {
			"$(WXWIN)/lib/vc_x64_dll"
		}

	filter "configurations:*32"
		libdirs {
			"$(WXWIN)/lib/vc_dll"
		}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"WXUSINGDLL",
			"GL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug*"
		symbols "On"
		defines { "GL_CONFIG_DEBUG" }

	filter {"system:windows", "configurations:Debug64"}
		postbuildcommands {
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxbase314ud_vc_x64_custom.dll\" %{cfg.targetdir}"),
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxmsw314ud_core_vc_x64_custom.dll\" %{cfg.targetdir}"),
		}

	filter {"system:windows", "configurations:Debug32"}
		postbuildcommands {
			("{COPY} \"$(WXWIN)/lib/vc_dll/wxbase314ud_vc_custom.dll\" %{cfg.targetdir}"),
			("{COPY} \"$(WXWIN)/lib/vc_dll/wxmsw314ud_core_vc_custom.dll\" %{cfg.targetdir}"),
		}

	filter "configurations:Release*"
		optimize "On"
		defines { "GL_CONFIG_RELEASE" }

	filter {"system:windows", "configurations:Release64"}
		postbuildcommands {
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxbase314u_vc_x64_custom.dll\" %{cfg.targetdir}"),
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxmsw314u_core_vc_x64_custom.dll\" %{cfg.targetdir}"),
		}

	filter {"system:windows", "configurations:Release32"}
		postbuildcommands {
			("{COPY} \"$(WXWIN)/lib/vc_dll/wxbase314u_vc_custom.dll\" %{cfg.targetdir}"),
			("{COPY} \"$(WXWIN)/lib/vc_dll/wxmsw314u_core_vc_custom.dll\" %{cfg.targetdir}"),
		}

project "Updater"
	location "Updater"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir  ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"GL_PLATFORM_WINDOWS",
			"GL_BUILD_DLL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Glipus");
		}

	filter "configurations:Debug*"
		symbols "On"
		defines { "GL_CONFIG_DEBUG" }

	filter "configurations:Release*"
		optimize "On"
		defines { "GL_CONFIG_RELEASE" }