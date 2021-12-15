workspace "gmsv_gsimd"
	configurations { "Debug32", "Debug64", "Release32", "Release64" }
	platforms { "sse", "avx128", "avx256", "avx512" }
	location ( "projects/" .. os.get() )

project "gmsv_gsimd"
	kind         "SharedLib"
	language     "C++"
	cppdialect	"C++11"
	editandcontinue "Off"
	vectorextensions "AVX"
	includedirs  "../include/"
	targetprefix ""
	targetextension ".dll"
	staticruntime "Off"
	floatingpoint "Fast"
	files
	{
		"src/**.*",
		"../include/**.*"
	}

	filter "configurations:Debug32"
		architecture "x86"
		symbols	"On"
		optimize "Debug"
		if os.is( "windows" ) then targetsuffix "_win32" end
		if os.is( "macosx" )  then targetsuffix "_osx"   end
		if os.is( "linux" )   then targetsuffix "_linux" end

	filter "configurations:Debug64"
		architecture "x86_64"
		symbols	"On"
		optimize "Debug"
		if os.is( "windows" ) then targetsuffix "_win64" end
		if os.is( "macosx" )  then targetsuffix "_osx64"   end
		if os.is( "linux" )   then targetsuffix "_linux64" end

	filter "configurations:Release32"
		architecture "x86"
		symbols	"Off"
		optimize "Speed"
		omitframepointer "On"
		flags { "LinkTimeOptimization" }
		if os.is( "windows" ) then targetsuffix "_win32" end
		if os.is( "macosx" )  then targetsuffix "_osx"   end
		if os.is( "linux" )   then targetsuffix "_linux" end

	filter "configurations:Release64"
		architecture "x86_64"
		symbols	"Off"
		optimize "Speed"
		omitframepointer "On"
		flags { "LinkTimeOptimization" }
		if os.is( "windows" ) then targetsuffix "_win64" end
		if os.is( "macosx" )  then targetsuffix "_osx64"   end
		if os.is( "linux" )   then targetsuffix "_linux64" end

	filter "platforms:sse"
		buildoptions { "-DSIMDPP_ARCH_X86_SSE4_1" }

	filter "platforms:avx128"
		buildoptions { "-DSIMDPP_ARCH_X86_SSE4_1", "-DSIMDPP_ARCH_X86_AVX" }

	filter "platforms:avx256"
		buildoptions { "-DSIMDPP_ARCH_X86_FMA3", "-DSIMDPP_ARCH_X86_AVX2" }

	filter "platforms:avx512"
		buildoptions { "-DSIMDPP_ARCH_X86_FMA3", "-DSIMDPP_ARCH_X86_AVX512BW", "-DSIMDPP_ARCH_X86_AVX512DQ", "-DSIMDPP_ARCH_X86_AVX512VL" }