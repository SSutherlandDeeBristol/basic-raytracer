set(sources SDLWrapper.h SDLWrapper.cpp)
add_library(SDL STATIC ${sources})

find_package(SDL2 REQUIRED)

target_include_directories(SDL PUBLIC "." ${SDL2_INCLUDE_DIRS})
target_link_libraries(SDL PRIVATE glm ${SDL2_LIBRARIES})