project "Core"
	language "C++"
	kind "SharedLib"
	targetname "core"
    targetdir (projbuild())
    objdir (projinter())

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
	
	includedirs {
		path.join(SOLUTION_DIR, 'Server')
	}