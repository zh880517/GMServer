
local BuilPath = "Build/" .. _ACTION  --工程路径
local ObjPath = "../ObjOut/" .. _ACTION  -- 编译中间文件输出目录
local OutPath = "%{cfg.buildcfg}_%{cfg.platform}"

workspace "Server"
	configurations {
		"Debug", 
		"Release" 
	}--项目配置类型
    platforms { "X32", "X64"}
    location(BuilPath)--解决方案目录

    flags {
		"C++11", 
		"MultiProcessorCompile"
	}
	
	filter "configurations:Debug" --编译配置
		defines { "DEBUG" } --预编译宏
		flags { "Symbols" } -- 生成pdb文件

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On" --开启优化选项

	group "ThirdParty"
   
		project "libprotobuf"
			kind "StaticLib" 
			language "C++" 
			targetdir("lib/"..OutPath) --生成文件目录
			location(BuilPath.."/ThirdParty/libprotobuf") --工程目录
			objdir(ObjPath.."/ThirdParty/libprotobuf/%{cfg.buildcfg}")
			includedirs {
				"ThirdParty/protobuf-3.0.2/src/"
			}
			configuration "windows"
				defines{"_DEBUG", "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS", "WIN32", "LIBPROTOBUF_EXPORTS", "_WINDOWS", "_USRDLL"}
			local pbPath = "ThirdParty/protobuf-3.0.2/src/google/protobuf/"
			files {
				pbPath.."any.cc", pbPath.."any.pb.cc", pbPath.."api.pb.cc", pbPath.."compiler/importer.cc", pbPath.."compiler/parser.cc", 
				pbPath.."descriptor.cc", pbPath.."descriptor.pb.cc", pbPath.."descriptor_database.cc", pbPath.."duration.pb.cc", pbPath.."dynamic_message.cc"
				, pbPath.."empty.pb.cc", pbPath.."extension_set_heavy.cc", pbPath.."field_mask.pb.cc", pbPath.."generated_message_reflection.cc"
				, pbPath.."io/gzip_stream.cc", pbPath.."io/printer.cc", pbPath.."io/strtod.cc", pbPath.."io/tokenizer.cc", pbPath.."io/zero_copy_stream_impl.cc"
				, pbPath.."map_field.cc", pbPath.."message.cc", pbPath.."reflection_ops.cc", pbPath.."service.cc", pbPath.."source_context.pb.cc"
				, pbPath.."struct.pb.cc", pbPath.."stubs/mathlimits.cc", pbPath.."stubs/substitute.cc", pbPath.."text_format.cc", pbPath.."timestamp.pb.cc"
				, pbPath.."type.pb.cc", pbPath.."unknown_field_set.cc", pbPath.."util/field_comparator.cc", pbPath.."util/field_mask_util.cc"
				, pbPath.."util/internal/datapiece.cc", pbPath.."util/internal/default_value_objectwriter.cc", pbPath.."util/internal/error_listener.cc", pbPath.."util/internal/field_mask_utility.cc"
				, pbPath.."util/internal/json_escaping.cc", pbPath.."util/internal/json_objectwriter.cc", pbPath.."util/internal/json_stream_parser.cc"
				, pbPath.."util/internal/object_writer.cc", pbPath.."util/internal/proto_writer.cc", pbPath.."util/internal/protostream_objectsource.cc"
				, pbPath.."util/internal/protostream_objectwriter.cc", pbPath.."util/internal/type_info.cc", pbPath.."util/internal/type_info_test_helper.cc"
				, pbPath.."util/internal/utility.cc", pbPath.."util/json_util.cc", pbPath.."util/message_differencer.cc", pbPath.."util/time_util.cc"
				, pbPath.."util/type_resolver_util.cc", pbPath.."wire_format.cc", pbPath.."wrappers.pb.cc"
			}

		project "Detour"
			kind "StaticLib" 
			language "C++" 
			targetdir("lib/"..OutPath) --生成文件目录
			location(BuilPath.."/ThirdParty/Detour") --工程目录
			objdir(ObjPath.."/ThirdParty/Detour/%{cfg.buildcfg}")
			includedirs {
				"ThirdParty/recastnavigation/Detour/Include"
			}
			files { 
				"ThirdParty/recastnavigation/Detour/**.h",
				"ThirdParty/recastnavigation/Detour/**.cpp" 
			}

		project "hiredis"
			kind "StaticLib" 
			language "C++" 
			targetdir("lib/"..OutPath) --生成文件目录
			location(BuilPath.."/ThirdParty/hiredis") --工程目录
			objdir(ObjPath.."/ThirdParty/hiredis/%{cfg.buildcfg}")
			defines{"_WINSOCK_DEPRECATED_NO_WARNINGS"}
			files { "ThirdParty/hiredis_win/**.h", "ThirdParty/hiredis_win/**.cpp", "ThirdParty/hiredis_win/**.c"} --源码文件目录
			
	group "CoreLib"
		
		project "Math"
			kind "StaticLib" 
			language "C++" 
			targetdir("lib/"..OutPath) --生成文件目录
			location(BuilPath.."/CoreLib/Math") --工程目录
			objdir(ObjPath.."/CoreLib/Math/%{cfg.buildcfg}")
			includedirs {
				"CoreLib/Math"
			}
			files { 
				"CoreLib/Math/**.h",
				"CoreLib/Math/**.cpp" 
			}
			
		project "Utils"
			kind "StaticLib" 
			language "C++" 
			targetdir("lib/"..OutPath) --生成文件目录
			location(BuilPath.."/CoreLib/Utils") --工程目录
			objdir(ObjPath.."/CoreLib/Utils/%{cfg.buildcfg}")
			includedirs {
				"CoreLib/Utils",
				"ThirdParty/rapidxml",
				"ThirdParty/MySql/include"
			}
			files { 
				"CoreLib/Utils/**.h",
				"CoreLib/Utils/**.cpp" 
			}
		
		project "LibActor"
			kind "StaticLib" 
			language "C++" 
			targetdir("lib/"..OutPath) --生成文件目录
			location(BuilPath.."/CoreLib/LibActor") --工程目录
			objdir(ObjPath.."/CoreLib/LibActor/%{cfg.buildcfg}")
			includedirs {
				"CoreLib/",
				"ThirdParty/boost_1_61_0"
			}
			files { 
				"CoreLib/LibActor/**.h",
				"CoreLib/LibActor/**.cpp" 
			}