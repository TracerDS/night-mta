project 'Client SDK'
	language 'C++'
	kind 'StaticLib'
	targetname 'sdk'

	vpaths {
		['Headers/*'] = { '**.hpp' },
		['*'] = 'premake5.lua'
	}

	files {
		'premake5.lua',
		'**.hpp',
	}

	flags { 'ExcludeFromBuild' }
