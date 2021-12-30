set(SOURCES main.cpp)
add_executable(TestApp main.cpp)
target_link_libraries(TestApp PUBLIC Camera Geometry SDL STD)