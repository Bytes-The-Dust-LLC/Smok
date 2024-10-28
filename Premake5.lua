include "../BTDSTD3"

project "Smok"
kind "StaticLib"
language "C++"
targetdir ("bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/BTDSDK")
objdir ("bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/BTDSDK")

files 
{
---base code
"./includes/**.h",
"./src/**.c",
"./includes/**.hpp",
"./src/**.cpp",

}

includedirs 
{
---base code
"./includes",

"../" .. BTD_INCLUDE,
"../BTDSTD3/" .. GLM_INCLUDE,
"../BTDSTD3/" .. FMT_INCLUDE,
"../BTDSTD3/" .. SDL_INCLUDE,

"../BTDSTD3/" .. VK_BOOTSTRAP_INCLUDE,
"../BTDSTD3/" .. STB_INCLUDE,
"../BTDSTD3/" .. VOLK_INCLUDE,
"../BTDSTD3/" .. VMA_INCLUDE,
VULKAN_SDK_MANUAL_OVERRIDE,
}

links
{
"BTDSTD",
}


defines
{
"GLM_FORCE_DEPTH_ZERO_TO_ONE",
"GLM_FORCE_RADIANS",
"GLM_ENABLE_EXPERIMENTAL",
}


flags
{
"MultiProcessorCompile",
"NoRuntimeChecks",
}


buildoptions
{
"/utf-8",
}


--platforms
filter "system:windows"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"


defines
{
"Window_Build",
"VK_USE_PLATFORM_WIN32_KHR",
"Desktop_Build",
}

filter "system:linux"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"


defines
{
"Linux_Build",
"VK_USE_PLATFORM_XLIB_KHR",
"Desktop_Build",
}


    filter "system:mac"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"


defines
{
"MacOS_Build",
"VK_USE_PLATFORM_MACOS_MVK",
"Desktop_Build",
}

--configs
filter "configurations:Debug"
    defines "BTD_DEBUG"
    symbols "On"

filter "configurations:Release"
    defines "BTD_RELEASE"
    optimize "On"


filter "configurations:Dist"
    defines "BTD_DIST"
    optimize "On"


defines
{
"NDEBUG",
}


flags
{
"LinkTimeOptimization",
}