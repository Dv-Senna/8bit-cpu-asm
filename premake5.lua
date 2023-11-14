workspace "assembler"
	configurations {"debug", "release"}

project "assembler"
	kind "ConsoleApp"
	language "C++"
	files {
		"src/**.hpp",
		"src/**.inl",
		"src/**.cpp"
	}
	cppdialect "C++20"

	targetdir "bin/%{cfg.buildcfg}"
	targetname "assembler"


	filter {"configurations:debug"}
		defines {"DEBUG"}
		symbols "On"
		warnings "Extra"

	filter {"configurations:release"}
		defines {"NDEBUG"}
		optimize "On"