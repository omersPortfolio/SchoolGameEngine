-----------------------------------------------------
-- Workspace
-----------------------------------------------------
workspace "Workspace"
	configurations	{ "Debug", "Release" }
	platforms		{ "x64" }
	location		"build"
	characterset	"MBCS"
	startproject	"Game"

	filter "configurations:Debug"
		symbols		"on"

-----------------------------------------------------
-- Game Project
-----------------------------------------------------
project "Game"
	kind		"WindowedApp"
	location	"build/Game"
	debugdir	"Game"
	cppdialect "C++17"									-- Changing language standard to support std::filesystem

	files {
		"Game/Source/**.cpp",
		"Game/Source/**.h",
		"Game/Data/**.vert",
		"Game/Data/**.frag",
		"Game/Data/**.scene",
		"premake5.lua",
		".gitignore",
		"GenerateProjectFiles.bat",
		"readme.txt",
		".editorconfig",
	}

	includedirs {
		"Game/Source",
	}

	links {
		"Framework",
		"opengl32",
	}


	pchheader "GamePCH.h"
	pchsource "Game/Source/WinMain.cpp"

-----------------------------------------------------
-- Framework Project
-----------------------------------------------------
project "Framework"
	kind		"StaticLib"
	location	"build/Framework"
	cppdialect "C++17"									-- Changing language standard to support std::filesystem

	files {
		"Framework/Source/**.cpp",
		"Framework/Source/**.h",
		"Framework/Libraries/imgui/*.cpp",
		"Framework/Libraries/imgui/*.h",
		"Framework/Libraries/stb/stb_image.h",
		"Framework/Libraries/rapidjson/include/rapidjson/**.h",
		"Framework/Libraries/box2d/include/**.h",
		"Framework/Libraries/box2d/src/**.cpp",
		"Framework/Libraries/box2d/src/**.h",
		".editorconfig",
	}

	includedirs {
		"Framework/Source",
		"Framework/Libraries/box2d/include",
		"Framework/Libraries/box2d/src",
	}

	pchheader "FrameworkPCH.h"
	pchsource "Framework/Source/Core/FWCore.cpp"

	filter "files:Framework/Libraries/imgui/*.cpp"
		flags { "NoPCH" }

	filter "files:Framework/Libraries/box2d/src/**.cpp"
		flags { "NoPCH" }

	