SOLUTION_DIR = path.join('${wks.location}','../')
CONFIGURATION = '%{cfg.buildcfg}'
PLATFORM = '%{cfg.platform}'
PROJECT_NAME = '%{prj.name}'

if os.host() == 'windows' then
    SOLUTION_DIR = '$(SolutionDir)..'
    CONFIGURATION = '$(Configuration)'
    PLATFORM = '$(PlatformTarget)'
    PROJECT_NAME = '$(ProjectName)'
end

function buildpath(p) return "%{wks.location}/../Bin/"..p.."/" end
function objpath(p) return "%{wks.location}/../Build/"..p.."/" end

workspace 'NightMTA'
    configurations { 'Debug', 'Release' }
    platforms { 'x86', 'x64' }

    cppdialect 'C++latest'
    cdialect 'C17'
    location 'Solutions'

    filter 'platforms:x86'
        architecture 'x86'
    filter 'platforms:x64'
        architecture 'x86_64'

    characterset 'MBCS'
    pic 'On'
    symbols 'On'
    flags 'MultiProcessorCompile'

	includedirs {
		path.join(SOLUTION_DIR)
	}

    filter 'configurations:Debug'
        defines { 'MTA_DEBUG', '_DEBUG' }
        targetsuffix '_d'
        runtime 'Debug'

    filter 'configurations:Release'
        optimize 'Full'
        runtime 'Release'

    filter 'system:windows'
        defines { '_WIN32' }
        preferredtoolarchitecture 'x86_64'
        staticruntime 'On'
        buildoptions { '/Zc:__cplusplus' }
    
	filter {}
        group 'Server'
        include 'Server/Core'
        include 'Server/Launcher'

        group 'Vendors'
        

        group 'Shared'
        include 'Shared'