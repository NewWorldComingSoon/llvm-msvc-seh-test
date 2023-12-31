# This file is automatically generated from cmake.toml - DO NOT EDIT
# See https://github.com/build-cpp/cmkr for more information

cmake_minimum_required(VERSION 3.15)

# Enable support for MSVC_RUNTIME_LIBRARY
cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-tree builds are not supported. Run CMake from a separate directory: cmake -B build")
endif()

set(CMKR_ROOT_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(CMKR_ROOT_PROJECT ON)

	# Bootstrap cmkr and automatically regenerate CMakeLists.txt
	include(cmkr.cmake OPTIONAL RESULT_VARIABLE CMKR_INCLUDE_RESULT)
	if(CMKR_INCLUDE_RESULT)
		cmkr()
	endif()

	# Enable folder support
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	# Create a configure-time dependency on cmake.toml to improve IDE support
	configure_file(cmake.toml cmake.toml COPYONLY)
endif()

project(llvm-msvc-seh-test)

include(FetchContent)

# Fix warnings about DOWNLOAD_EXTRACT_TIMESTAMP
if(POLICY CMP0135)
	cmake_policy(SET CMP0135 NEW)
endif()
message(STATUS "Fetching googletest (v1.13.0)...")
FetchContent_Declare(googletest SYSTEM
	GIT_REPOSITORY
		"https://github.com/google/googletest"
	GIT_TAG
		v1.13.0
)
FetchContent_MakeAvailable(googletest)

# Target: test_seh
set(test_seh_SOURCES
	"src/CXXEH.cpp"
	"src/SEH.cpp"
	"src/SEHWithEha.cpp"
	"src/main.cpp"
	cmake.toml
)

add_executable(test_seh)

target_sources(test_seh PRIVATE ${test_seh_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${test_seh_SOURCES})

target_compile_features(test_seh PRIVATE
	cxx_std_20
)

target_compile_options(test_seh PRIVATE
	"/EHa"
	-fauto-generate-ir
	-fprint-arguments
)

target_include_directories(test_seh PRIVATE
	"googletest/googletest/include"
)

target_link_libraries(test_seh PRIVATE
	gtest
)

get_directory_property(CMKR_VS_STARTUP_PROJECT DIRECTORY ${PROJECT_SOURCE_DIR} DEFINITION VS_STARTUP_PROJECT)
if(NOT CMKR_VS_STARTUP_PROJECT)
	set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT test_seh)
endif()

