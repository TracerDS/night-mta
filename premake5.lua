buildpath = function(p) return '%{wks.location}/../Bin/'..p..'/' end
copy = function(p) return '{COPY} %{cfg.buildtarget.abspath} "%{wks.location}../Bin/'..p..'/"' end
SolutionName = 'NightMTA'

workspace ( SolutionName )
	configurations { 'Debug', 'Release' }

	platforms { 'x86', 'x64' }

	targetprefix ''

	location 'Build'
	startproject 'Client Launcher'

	cppdialect 'C++latest'
	characterset 'MBCS'
	pic 'On'
	symbols 'On'
	flags 'MultiProcessorCompile'

	dxdir = os.getenv('DXSDK_DIR') or ''
	includedirs {
		'.',
		'Shared',
		'vendor',
	}

	filter 'platforms:x86'
		architecture 'x86'
	filter 'platforms:x64'
		architecture 'x86_64'

	filter 'configurations:Debug'
		defines { '_DEBUG', 'MTA_DEBUG' }
		targetsuffix '_d'
		runtime 'Debug'
		optimize 'Debug'

	filter 'configurations:Release'
		defines { 'NDEBUG' }
		runtime 'Release'
		optimize 'Speed' -- never 'Full'

	filter { 'system:windows', 'configurations:Debug' }
		symbolspath '$(SolutionDir)Symbols\\$(Configuration)_$(Platform)\\$(ProjectName).pdb'

	filter 'system:windows'
		toolset 'v143'
		preferredtoolarchitecture 'x86_64'
		staticruntime 'On'
		defines {
			'_WIN32',
			'WIN32_LEAN_AND_MEAN',
			'NOMINMAX',
		}
		buildoptions { '/Zc:__cplusplus' }
		includedirs {
			path.join(dxdir, 'Include')
		}
		libdirs {
			path.join(dxdir, 'Lib/x86')
		}

	-- Only build the client on Windows
	if os.target() == 'windows' then
		group 'Client'
		include 'Client/Launcher'
		include 'Client/Loader'
		include 'Client/sdk'

		group 'Client/CEGUI'

		group 'Vendor'
	end

	filter {}
		group 'Server'

		group 'Shared'
		include 'Shared'

		group 'Vendor'
