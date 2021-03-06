workspace "i7w"
    architecture "x86_64"
    configurations 
    {
        "Debug",
        "Release"
    }

project "ecs"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    buildoptions { "-fdiagnostics-color=always -Wall -Wextra -Wno-unused-parameter" }

    files 
    { 
        "src/**.h", 
        "src/**.cpp",
    }

    includedirs 
    { 
        "src",
    }

    links
    {
    }

    filter { "configurations:Debug" }
        defines { "DEBUG", "VERBOSE" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"

