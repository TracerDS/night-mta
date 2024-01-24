project "Launcher"
	language "C++"
	kind "ConsoleApp"
	targetname "launcher"
    targetdir (projbuild())
    objdir (projinter())

	dependson { "Core" }

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
	libdirs {
		builddir('Core')
	}