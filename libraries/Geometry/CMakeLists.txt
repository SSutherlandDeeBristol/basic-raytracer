set(sources Geometry.h Geometry.cpp Scenes.h Scenes.cpp Material.cpp Material.h GeometryUtils.h GeometryUtils.cpp)
add_library(Geometry STATIC ${sources})
target_include_directories(Geometry PUBLIC ".")
target_link_libraries(Geometry PUBLIC Camera)