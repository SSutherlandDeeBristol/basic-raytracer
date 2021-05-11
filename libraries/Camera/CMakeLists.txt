set(sources Camera.h Camera.cpp GlmTypes.h)
add_library(Camera STATIC ${sources})

target_link_libraries(Camera PUBLIC glm)
target_include_directories(Camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})