workspace "Glipus_Project"
	configurations { "Debug", "Release" }
	platforms { "Win32", "Win64" }

	filter "platforms:Win32"
		system "Windows"
		architecture "x86"
		defines { "GL_ARCH_86" }

	filter "platforms:Win64"
		system "Windows"
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
		"%{prj.name}/src/Dialogs/*",
		"%{prj.name}/src/Utils/*",
		"%{prj.name}/**.rc",
		"%{prj.name}/assets/**.ico",
	}

	includedirs {
		"$(WXWIN)/include/msvc",
		"$(WXWIN)/include",
		"%{wks.location}/Updater/src/",
	}

	links {
		"Updater"
	}

	filter "platforms:Win64"
		libdirs {
			"$(WXWIN)/lib/vc_x64_dll",
		}

	filter "platforms:Win32"
		libdirs {
			"$(WXWIN)/lib/vc_dll",
		}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"WXUSINGDLL",
			"GL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		symbols "On"
		defines { "GL_CONFIG_DEBUG" }

	filter { "platforms:Win64", "configurations:Debug" }
		postbuildcommands {
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxbase314ud_vc_x64_custom.dll\" %{cfg.targetdir}"),
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxmsw314ud_core_vc_x64_custom.dll\" %{cfg.targetdir}"),
		}

	filter { "platforms:Win32", "configurations:Debug"}
		postbuildcommands {
			("{COPY} \"$(WXWIN)/lib/vc_dll/wxbase314ud_vc_custom.dll\" %{cfg.targetdir}"),
			("{COPY} \"$(WXWIN)/lib/vc_dll/wxmsw314ud_core_vc_custom.dll\" %{cfg.targetdir}"),
		}

	filter "configurations:Release"
		optimize "On"
		defines { "GL_CONFIG_RELEASE" }

	filter { "platforms:Win64", "configurations:Release" }
		postbuildcommands {
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxbase314u_vc_x64_custom.dll\" %{cfg.targetdir}"),
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxmsw314u_core_vc_x64_custom.dll\" %{cfg.targetdir}"),
		}

	filter { "platforms:Win32", "configurations:Release" }
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
		"%{prj.name}/src/**.h",
		"%{prj.name}/**.rc"
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

	filter "configurations:Debug"
		symbols "On"
		defines { "GL_CONFIG_DEBUG" }

	filter "configurations:Release"
		optimize "On"
		defines { "GL_CONFIG_RELEASE" }