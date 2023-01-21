cmake_minimum_required (VERSION 3.5)
project(test)
set (CMAKE_CXX_STANDARD 11)
find_package( OpenCV 4.0.0 REQUIRED )

add_executable(phone_webcam src/phone_webcam.cpp)
target_link_libraries(phone_webcam ${OpenCV_LIBS})