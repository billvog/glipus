workspace "Glipus"
	architecture "x64"

	configurations {
		"Debug",
		"Release"
	}

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

	libdirs {
		"$(WXWIN)/lib/vc_x64_dll"
	}

	links {
		"Updater"
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

	filter {"system:windows", "configurations:Debug"}
		postbuildcommands {
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxbase314ud_vc_x64_custom.dll\" %{cfg.targetdir}"),
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxmsw314ud_core_vc_x64_custom.dll\" %{cfg.targetdir}"),
		}

	filter "configurations:Release"
		optimize "On"

	filter {"system:windows", "configurations:Release"}
		postbuildcommands {
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxbase314u_vc_x64_custom.dll\" %{cfg.targetdir}"),
			("{COPY} \"$(WXWIN)/lib/vc_x64_dll/wxmsw314u_core_vc_x64_custom.dll\" %{cfg.targetdir}"),
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

	filter "configurations:Release"
		optimize "On"