project 'Loader'
	language 'C++'
	kind 'SharedLib'
	targetname 'loader'
	targetdir(buildpath('mta'))

	filter 'system:windows'
        -- https://stackoverflow.com/questions/25081033/what-safesehno-option-actually-do
		linkoptions { '/SAFESEH:NO' }

	includedirs {
		'../sdk',
	}

	links {
        
	}

	--pchheader 'StdInc.hpp'
	--pchsource 'StdInc.cpp'
	--forceincludes { 'StdInc.hpp' }

	vpaths {
		['Headers/*'] = '**.hpp',
		['Sources'] = '**.cpp',
		['Resources/*'] = { '**.rc', '**.bmp' },
		['*'] = 'premake5.lua'
	}

	files {
		'premake5.lua',
		'**.hpp',
		'**.cpp'
	}

	filter 'system:windows'
		files {
			'loader.rc',
			'resource/splash.bmp'
		}

	filter 'system:not windows'
		flags { 'ExcludeFromBuild' }
