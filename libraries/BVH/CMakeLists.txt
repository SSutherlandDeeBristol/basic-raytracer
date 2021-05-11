set(sources BVH.h BVH.cpp)
add_library(BVH STATIC ${sources})
target_include_directories(BVH PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(BVH PUBLIC Camera)