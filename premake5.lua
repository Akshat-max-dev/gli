project "Gli"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "gli/**.hpp"
        "gli/core/**.hpp",
        "gli/core/**.inl"
    }

    includedirs
    {
        "gli",
        "gli/core"
    }

    filter "system:windows"
            systemversion "latest"

    filter "configurations:Debug"
            defines "DEBUG"
            runtime "Debug"
            symbols "on"

    filter "configurations:Release"
            defines "RELEASE"
            runtime "Release"
            optimize "on"