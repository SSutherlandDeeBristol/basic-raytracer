cmake_minimum_required(VERSION 3.1)
project(basic-raytracer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("external")

add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-std=c++17>")
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wall>")
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wextra>")
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Werror>")
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wno-unknown-pragmas>")
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wno-unused-but-set-variable>")

add_subdirectory("apps")
add_subdirectory("libraries")