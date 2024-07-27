project 'Client Launcher'
	language 'C++'
	--kind 'WindowedApp'
	kind 'ConsoleApp'
	targetname (SolutionName)
	targetdir(buildpath('.'))
	debugdir(buildpath('.'))

    defaultplatform 'x64'

	includedirs {
		'../../Shared/sdk',
	}

	vpaths {
		['Headers/*'] = '**.hpp',
		['Sources/*'] = '**.cpp',
		['Resources/*'] = {'*.rc', '**.ico', '**.xml', '**.manifest'},
		['*'] = 'premake5.lua'
	}

	files {
		'premake5.lua',
		'*.hpp',
		'*.cpp'
	}

	filter 'system:windows'
		staticruntime 'On'

	filter 'system:not windows'
		flags { 'ExcludeFromBuild' }
