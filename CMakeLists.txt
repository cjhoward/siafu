cmake_minimum_required(VERSION 3.21)
project(siafu LANGUAGES CXX)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the source files in the "src" directory
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)

# Create an executable using the specified source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Set executable compile options
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -pedantic -Werror -Wconversion -Wshadow -flto>
    $<$<CXX_COMPILER_ID:MSVC>:/MP /MT /GS- /W4 /WX /GL>
)

# Set executable link options
target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-static -flto>
	$<$<CXX_COMPILER_ID:MSVC>:/LTCG>
)

# Set release build-specific compile and link options
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_options(${PROJECT_NAME} PRIVATE
		$<$<CXX_COMPILER_ID:GNU,Clang>:-O3 -march=native>
		$<$<CXX_COMPILER_ID:MSVC>:/Ox /Ob2 /Ot /fp:fast /Z7>
	)
	target_link_options(${PROJECT_NAME} PRIVATE
		$<$<CXX_COMPILER_ID:GNU,Clang>:-s>
		$<$<CXX_COMPILER_ID:MSVC>:/NOLOGO /OPT:REF /OPT:ICF /INCREMENTAL:NO>
	)
endif()

# Place the generated .exe directly in the bin directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)
