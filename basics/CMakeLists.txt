cmake_minimum_required (VERSION 3.5)
project(basics)
set (CMAKE_CXX_STANDARD 11)
find_package( OpenCV 4.0.0 REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(changeConBri src/changeConBri.cpp)
target_link_libraries(changeConBri ${OpenCV_LIBS})

add_executable(threshold src/threshold.cpp)
target_link_libraries(threshold ${OpenCV_LIBS})

add_executable(canny src/canny.cpp)
target_link_libraries(canny ${OpenCV_LIBS})

add_executable(structs src/structs.cpp)
target_link_libraries(structs ${OpenCV_LIBS})