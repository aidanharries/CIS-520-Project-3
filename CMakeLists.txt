cmake_minimum_required (VERSION 2.8)
project(hw3)

set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror -D_XOPEN_SOURCE=500")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror -Wno-sign-compare -D_XOPEN_SOURCE=500")

include_directories("${PROJECT_SOURCE_DIR}/include")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build a dynamic library called libblock_store.so
# note that the prefix lib will be automatically added in the filename.
set(SOURCE_FILES src/block_store.c src/bitmap.c)
add_library(block_store SHARED ${SOURCE_FILES})

# make an executable
add_executable(${PROJECT_NAME}_test test/tests.cpp)
target_compile_definitions(${PROJECT_NAME}_test PRIVATE)
target_link_libraries(${PROJECT_NAME}_test gtest pthread block_store)
