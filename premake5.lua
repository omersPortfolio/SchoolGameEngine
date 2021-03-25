-----------------------------------------------------
-- Config
-----------------------------------------------------
PremakeConfig_UseLua = false

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
	cppdialect	"C++17"									-- Changing language standard to support std::filesystem

	files {
		"Game/Source/**.cpp",
		"Game/Source/**.h",
		"Game/Source/**.lua",
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
		"Framework/Libraries",
		"Framework/Libraries/LuaBridge/Source",
	}

	links {
		"Framework",
		"opengl32",
		"xinput",
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
		"Framework/Libraries/box2d/include/**.h",
		"Framework/Libraries/box2d/src/**.cpp",
		"Framework/Libraries/box2d/src/**.h",
		"Framework/Libraries/imgui/*.cpp",
		"Framework/Libraries/imgui/*.h",
		"Framework/Libraries/ImFileDialog/ImFileDialog.cpp",
		"Framework/Libraries/ImFileDialog/ImFileDialog.h",
		"Framework/Libraries/Lua/src/**.c",
		"Framework/Libraries/Lua/src/**.h",
		"Framework/Libraries/LuaBridge/**.h",
		"Framework/Libraries/rapidjson/include/rapidjson/**.h",
		"Framework/Libraries/stb/stb_image.h",
		".editorconfig",
	}

	includedirs {
		"Framework/Source",
		"Framework/Libraries",
		"Framework/Libraries/box2d/include",
		"Framework/Libraries/box2d/src",
		"Framework/Libraries/ImFileDialog",
		"Framework/Libraries/LuaBridge/Source",
		"Framework/Libraries/stb",
	}

	pchheader "FrameworkPCH.h"
	pchsource "Framework/Source/Core/FWCore.cpp"

	filter "files:Framework/Libraries/box2d/src/**.cpp"
		flags { "NoPCH" }

	filter "files:Framework/Libraries/imgui/*.cpp"
		flags { "NoPCH" }

	filter "files:Framework/Libraries/ImFileDialog/*.cpp"
		flags { "NoPCH" }

	filter { "files:Framework/Libraries/Lua/src/lua.c"
			.. " or Framework/Libraries/Lua/src/luac.c"
		   }
		   flags	"ExcludeFromBuild"

	filter "files:Framework/Libraries/Lua/**.c"
		flags { "NoPCH" }

	filter "files:Framework/Libraries/LuaBridge/**.h"
		flags { "NoPCH" }

if PremakeConfig_UseLua == true then
	filter {}
		defines			"FW_USE_LUA"
else
	filter {}
		defines			"FW_USE_LUA=0"
	filter{ "files:Framework/Libraries/Lua/**" }
		flags			"ExcludeFromBuild"
end
