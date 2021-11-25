workspace "Example"
    configurations {"DebugLib","ReleaseLib", "DebugDLL", "ReleaseDLL"}
    platforms {"Win32"}

filter "platforms:Win32"
    architecture "x32"

project "Example"
    basedir "Intermediate/Example"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir "Binaries/Example/%{cfg.buildcfg}"
    objdir "Build/Example/%{cfg.buildcfg}"

    files {
        "Source/Example/Private/*",
        "Source/Example/Public/*"
    }

    includedirs {
        "Source/ACReader/Public/",
        "Source/Example/Public/"
    }

    links { "ACReader" }

    filter "configurations:DebugLib"
        defines {"DEBUG"}
        symbols "On"
        libdirs {"Binaries/Example/DebugLib/"}
        prebuildcommands { '{COPYFILE} "%{wks.location}Binaries/ACReader/DebugLib/ACReader.lib" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"',
                           '{COPYFILE} "%{wks.location}Binaries/ACReader/DebugLib/ACReader.pdb" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"' }
    
    filter "configurations:ReleaseLib"
        defines {"NDEBUG"}
        optimize "On"
        libdirs {"Binaries/Example/ReleaseLib/"}
        prebuildcommands { '{COPYFILE} "%{wks.location}Binaries/ACReader/ReleaseLib/ACReader.lib" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"',
                           '{COPYFILE} "%{wks.location}Binaries/ACReader/ReleaseLib/ACReader.pdb" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"' }
        
    filter "configurations:DebugDll"
        defines {"DEBUG", "DLL_IMPORT"}
        symbols "On"
        libdirs {"Binaries/Example/DebugDll/"}
        prebuildcommands { '{COPYFILE} "%{wks.location}Binaries/ACReader/DebugDLL/ACReader.dll" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"',
                            '{COPYFILE} "%{wks.location}Binaries/ACReader/DebugDLL/ACReader.lib" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"',
                            '{COPYFILE} "%{wks.location}Binaries/ACReader/DebugDLL/ACReader.pdb" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"' }

    filter "configurations:ReleaseDll"
        defines {"NDEBUG", "DLL_IMPORT"}
        optimize "On"
        libdirs {"Binaries/Example/ReleaseDll/"}
        prebuildcommands { '{COPYFILE} "%{wks.location}Binaries/ACReader/ReleaseDLL/ACReader.dll" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"',
                            '{COPYFILE} "%{wks.location}Binaries/ACReader/ReleaseDLL/ACReader.lib" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"',
                            '{COPYFILE} "%{wks.location}Binaries/ACReader/ReleaseDLL/ACReader.pdb" "%{wks.location}Binaries/Example/%{cfg.buildcfg}"' }

workspace "ACReader"
    configurations { "DebugLib", "DebugDLL", "ReleaseLib", "ReleaseDLL" }
    platforms {"Win32"}

    filter "platforms:Win32"
        architecture "x32"

        defines { "DLL_EXPORT" }

project "ACReader"
    basedir "Intermediate/ACReader"
    language "C++"
    cppdialect "C++17"
    pchheader "pch.h"
    pchsource "Source/ACReader/Private/pch.cpp"

    targetdir "Binaries/ACReader/%{cfg.buildcfg}"
    objdir "Build/ACReader/%{cfg.buildcfg}"

    files {
        "Source/ACReader/Private/*",
        "Source/ACReader/Public/*"
    }

    includedirs {
        "Source/ACReader/Public/"
    }

    filter "*Lib"
        kind "StaticLib"

    filter "*DLL"
        kind "SharedLib"

    filter "configurations:DebugLib"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:DebugDLL"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:ReleaseLib"
        defines {"NDEBUG"}
        optimize "On"

    filter "configurations:ReleaseDLL"
        defines {"NDEBUG"}
        optimize "On"
