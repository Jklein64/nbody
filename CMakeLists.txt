cmake_minimum_required(VERSION 3.19)

project(nbody
VERSION 1.0
DESCRIPTION "Parallel N-body simulation with MPI and the Barnes Hut Algorithm"
LANGUAGES CXX
)

include(FetchContent)

# (external) argparse library
FetchContent_Declare(argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG cbd9fd8ed675ed6a2ac1bd7142d318c6ad5d3462
)
FetchContent_MakeAvailable(argparse)

# (external) gif generation library
# see https://stackoverflow.com/a/65620000
FetchContent_Declare(gif
    GIT_REPOSITORY https://github.com/charlietangora/gif-h.git
    GIT_TAG 70b645280d5e687f5217177c9cfa2889b0a2ad5f
)
FetchContent_MakeAvailable(gif)
add_library(gif INTERFACE)
if(NOT EXISTS ${gif_SOURCE_DIR}/gif/gif.hpp)
file(MAKE_DIRECTORY ${gif_SOURCE_DIR}/gif)
file(RENAME ${gif_SOURCE_DIR}/gif.h ${gif_SOURCE_DIR}/gif/gif.hpp)
endif()
target_include_directories(gif INTERFACE ${gif_SOURCE_DIR})

# (external) glm for vector types and operations
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

# library
file(GLOB lib_files CONFIGURE_DEPENDS lib/*.cpp)
add_library(lib ${lib_files})
target_include_directories(lib PUBLIC include)
target_compile_features(lib PUBLIC cxx_std_11)

# serial executable
file(GLOB src_files CONFIGURE_DEPENDS src/*.cpp src/*.h)
add_executable(nbody ${src_files})
target_link_libraries(nbody PUBLIC lib)
target_link_libraries(nbody PRIVATE argparse gif glm::glm)