project "Core"
	language "C++"
	kind "SharedLib"
	targetname "core"
    targetdir (buildpath('server'))
    objdir (objpath('server'))

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