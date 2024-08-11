project 'Client Launcher'
	language 'C++'
	kind 'WindowedApp'
	targetname 'NightMTA'
	targetdir(buildpath('.'))
	debugdir(buildpath('.'))

	includedirs {
		'..',
	}

	pchheader 'StdInc.hpp'
	pchsource 'StdInc.cpp'

	--entrypoint 'WinMainCRTStartup'

	vpaths {
		['Headers/*'] = '**.hpp',
		['Sources/*'] = '**.cpp',
		['Resources/*'] = { '**.rc', '**.ico', '**.xml', '**.manifest' },
		['*'] = 'premake5.lua'
	}

	files {
		'premake5.lua',
		'**.hpp',
		'**.cpp'
	}

	filter 'system:windows'
		staticruntime 'On'
		files {
			'NEU/gameicon_NEU.ico',
			'NEU/Multi Theft Auto.gdf.xml',
			'launch.rc',
			'Multi Theft Auto.rc',
			'Multi Theft Auto.manifest',
			'resource/mtaicon.ico'
		}

	filter 'system:not windows'
		flags { 'ExcludeFromBuild' }
