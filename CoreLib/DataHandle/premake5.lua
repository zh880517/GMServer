
local BuilPath = "Build"  --工程路径
local ObjPath = "../../CommonLibsOut/" .. _ACTION  -- 编译中间文件输出目录
local OutPath = "../../Lib"
workspace "DataHandle"
	configurations { "Debug", "Release" }--项目配置类型
    platforms { "X32", "X64"}
    location(BuilPath)--解决方案目录

    flags {"C++11", "MultiProcessorCompile", "StaticRuntime"}
	characterset("MBCS") --设置字符集
	filter "configurations:Debug" --编译配置
		defines { "_DEBUG" } --预编译宏
		flags { "Symbols" } -- 生成pdb文件

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On" --开启优化选项

	filter "platforms:*32" 
    	architecture "x86"
	filter "platforms:*64" 
    	architecture "x86_64"

    startproject "Test" 

project "hiredis"
    kind "StaticLib" 
    language "C++" 
    targetdir(OutPath) --生成文件目录
    location(BuilPath) --工程目录
    objdir(ObjPath.."/%{cfg.buildcfg}/hiredis")
	defines{"_WINSOCK_DEPRECATED_NO_WARNINGS"}
    files { "hiredis_win/**.h", "hiredis_win/**.cpp", "hiredis_win/**.c"} --源码文件目录
    

project "DataHandle"
    kind "StaticLib" 
    language "C++" 
    targetdir(OutPath) --生成文件目录
    location(BuilPath) --工程目录
    objdir(ObjPath.."/%{cfg.buildcfg}/DataHandle")
    --defines{"_CRT_SECURE_NO_WARNINGS"}
    includedirs {"RedisAdapter", "MySqlAdapter", "../", "./", "hiredis_win", "../Database/MySQL"} --头文件目录
    links {"hiredis"}
    libdirs {"../../Lib"}
    files { "RedisAdapter/**.h", "RedisAdapter/**.cpp", "TData/**.h", "MySqlAdapter/**.cpp", "MySqlAdapter/**.h", "DataTransfer/**.cpp", "DataTransfer/**.h"} --源码文件目录


project "Test"
    kind "ConsoleApp" 
    language "C++" 
    targetdir("bin/"..OutPath) --生成文件目录
    targetdir(OutPath) --生成文件目录
    location(BuilPath) --工程目录
    objdir(ObjPath.."/%{cfg.buildcfg}/Test")
    includedirs{ "../", "./", "Test", "../Database/MySQL"}
    dependson {"DataHandle"}
    libdirs {"../../Lib"}
    links{"DataHandle", "hiredis", "ws2_32", "libmysql"}
    files {"Test/**.h", "Test/**.cpp"}