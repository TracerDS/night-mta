project "Launcher"
	language "C++"
	kind "ConsoleApp"
	targetname "launcher"
    targetdir (buildpath('server'))
    objdir (objpath('server'))

	dependson { "Core", "SharedCore" }

    files {
		"premake5.lua",
		"*.hpp",
		"*.cpp"
	}

    vpaths {
		["Headers/*"] = "*.hpp",
		["Sources"] = "*.cpp",
		["*"] = "premake5.lua"
	}
	
	libdirs {
		buildpath('shared')
	}