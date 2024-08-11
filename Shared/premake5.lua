project "Shared"
	language "C++"
	kind "None"
	targetname "shared"

	vpaths {
		["Headers/*"] = { "**.hpp" },
		["*"] = "premake5.lua",
	}

	files {
		"premake5.lua",
		"**.hpp",
	}

	flags { "ExcludeFromBuild" }
