set(sources Geometry.h Geometry.cpp TestModel.h TestModel.cpp)
add_library(Geometry STATIC ${sources})
target_include_directories(Geometry PUBLIC ".")
target_link_libraries(Geometry PUBLIC Camera BVH)