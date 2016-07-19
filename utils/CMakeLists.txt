cmake_minimum_required (VERSION 2.8.11)
project(utils)

add_library(utils STATIC units.cc)



add_executable(test-unit test-unit.cc)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
include_directories(/home/lrleon/aleph-w .)

target_compile_options(test-unit PUBLIC -DDEBUG -D_GLIBCXX__PTHREADS -Wall -std=c++14 -O0 -g)

target_include_directories(test-unit INTERFACE ./)


