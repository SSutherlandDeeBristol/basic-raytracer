set(sources SDLauxiliary.h SDLauxiliary.cpp)
add_library(SDL STATIC ${sources})
target_include_directories(SDL PUBLIC ".")
target_link_libraries(SDL PRIVATE glm SDL2)