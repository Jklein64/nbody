cmake_minimum_required(VERSION 3.19)

project(nbody
    VERSION 1.0
    DESCRIPTION "Parallel N-body simulation with MPI and the Barnes Hut Algorithm"
    LANGUAGES CXX
)

# library
file(GLOB lib_files lib/*.cpp)
add_library(lib ${lib_files})
target_include_directories(lib PUBLIC include)
target_compile_features(lib PUBLIC cxx_std_11)

# serial executable
file(GLOB src_files src/*.cpp src/*.h)
add_executable(nbody ${src_files})
target_link_libraries(nbody PUBLIC lib)