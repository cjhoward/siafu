# SPDX-FileCopyrightText: 2023 C. J. Howard
# SPDX-License-Identifier: CC0-1.0

cmake_minimum_required(VERSION 3.27)

# Init project
project(Siafu
	VERSION 1.0.0
	DESCRIPTION "Isosurface extraction utility"
	HOMEPAGE_URL "https://github.com/cjhoward/siafu"
	LANGUAGES CXX
)
string(TOLOWER "${PROJECT_NAME}" PROJECT_SLUG)
set(PROJECT_AUTHOR "C. J. Howard")

# Specify the source files in the "src" directory
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
	${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Generate config header
configure_file(${PROJECT_SOURCE_DIR}/src/config.hpp.in ${PROJECT_BINARY_DIR}/src/config.hpp)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	
	# Generate Windows version-information resource file
	configure_file(${PROJECT_SOURCE_DIR}/res/windows/version.rc.in ${PROJECT_BINARY_DIR}/res/windows/version.rc)
	list(APPEND SOURCE_FILES "${PROJECT_BINARY_DIR}/res/windows/version.rc")
	
endif()

# Create an executable using the specified source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Set executable properties
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		OUTPUT_NAME $<LOWER_CASE:${PROJECT_NAME}>
		COMPILE_WARNING_AS_ERROR ON
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# Set executable include directories
target_include_directories(${PROJECT_NAME}
	PRIVATE
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_BINARY_DIR}/src
)

# Set executable compile options
target_compile_options(${PROJECT_NAME}
	PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:
			/W4 /EHsc /GR-
			$<$<NOT:$<CONFIG:Debug>>:/GL>
			$<$<CONFIG:Release>:/O2 /Ob3 /Qfast_transcendentals /GS->
		>
		$<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -pedantic>
)

# Set executable link options
target_link_options(${PROJECT_NAME} PRIVATE
	PRIVATE
		$<$<PLATFORM_ID:Windows>:
			$<$<CXX_COMPILER_ID:MSVC>:
				$<$<NOT:$<CONFIG:Debug>>:/OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG>
			>
		>
		$<$<CXX_COMPILER_ID:GNU,Clang>:
			-static
			$<$<NOT:$<CONFIG:Debug>>:-flto>
		>
)

# Install executable
install(TARGETS ${PROJECT_NAME})
