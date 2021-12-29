set(sources ThreadPool.h ThreadPool.cpp Latch.cpp Latch.h)
add_library(STD STATIC ${sources})
target_include_directories(STD PUBLIC ".")