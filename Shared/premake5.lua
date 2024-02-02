project "Shared"
	language "C++"
	kind "None"
	targetname "shared"

    files {
		"premake5.lua",
		"**.hpp",
		"**.cpp"
	}

    vpaths {
		["Headers/*"] = "**.hpp",
		["*"] = "premake5.lua"
	}
	
	flags { "ExcludeFromBuild" }