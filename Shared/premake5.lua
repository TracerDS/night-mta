-- Shared/premake5.lua
project 'Shared'
	language 'C++'
	cppdialect 'C++latest'
	kind 'None'
	targetname 'shared'

	vpaths {
		['Headers/*'] = { '**.hpp' },
		['*'] = 'premake5.lua',
	}

	files {
		'premake5.lua',
		'**.h',
		'**.hpp',
	}

	flags { 'ExcludeFromBuild' }
