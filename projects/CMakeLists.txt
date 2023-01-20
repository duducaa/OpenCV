cmake_minimum_required (VERSION 3.5)
project(io_tests)
set (CMAKE_CXX_STANDARD 11)
find_package( OpenCV 4.0.0 REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(shapeClassifier src/shapeClassifier.cpp)
target_link_libraries(shapeClassifier ${OpenCV_LIBS})

add_executable(colorDetection src/colorDetection.cpp)
target_link_libraries(colorDetection ${OpenCV_LIBS})

add_executable(bookFinder src/bookFinder.cpp)
target_link_libraries(bookFinder ${OpenCV_LIBS})

add_executable(vignette src/vignette.cpp)
target_link_libraries(vignette ${OpenCV_LIBS})

add_executable(moneyCount src/moneyCount.cpp)
target_link_libraries(moneyCount ${OpenCV_LIBS})

add_executable(objInfo src/objInfo.cpp)
target_link_libraries(objInfo ${OpenCV_LIBS})