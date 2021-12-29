set(SOURCES test.cpp)
add_executable(TestApp test.cpp)
target_link_libraries(TestApp PUBLIC Camera Geometry SDL STD)