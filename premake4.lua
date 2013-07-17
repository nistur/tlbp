solution "tlbp"
language "C++"
configurations { "Debug", "Release" }
includedirs { "include", "src/include" }
files { "include/**.h" }

defines { "TLBP_BUILD" }

configuration "Debug"
defines { "DEBUG" }
flags { "Symbols" }
targetdir "build/debug"

configuration "Release"
defines { "NDEBUG" }
flags { "OptimizeSpeed",
	"ExtraWarnings",
	"FatalWarnings",
	"NoFramePointer" }
targetdir "build/release"

project "tlbp"
kind "StaticLib"
files { "src/**.c", "src/**.cpp" }

project "tlbp-dynamic"
kind "SharedLib"
files { "src/**.c", "src/**.cpp" }
targetname "tlbp"

project "tests"
kind "ConsoleApp"
files { "tests/**.cpp" }
links { "tlbp" }
configuration "Debug"
postbuildcommands("build/debug/tests")
configuration "Release"
postbuildcommands("build/release/tests")
