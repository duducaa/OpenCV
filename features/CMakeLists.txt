cmake_minimum_required (VERSION 3.5)
project(basics)
set (CMAKE_CXX_STANDARD 11)
find_package( OpenCV 4.0.0 REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(stitch src/stitch.cpp)
target_link_libraries(stitch ${OpenCV_LIBS})
