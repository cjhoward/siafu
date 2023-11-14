cmake_minimum_required(VERSION 3.21)
project(siafu LANGUAGES CXX)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define build types and set default to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Enable all warnings and treat them as errors
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/MP /MT /GS- /W4 /WX)
endif()

# Specify the source files in the "src" directory
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)

# Create an executable using the specified source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Compiler-specific optimization flags for Release build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O3)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/Ox /Ob2)
		target_link_options(${PROJECT_NAME} PRIVATE /NOLOGO /OPT:REF /OPT:ICF /INCREMENTAL:NO)
    endif()
endif()

# Place the generated .exe directly in the bin directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)
