

find_package(OpenCV REQUIRED)

add_definitions(--std=c++11)
include_directories(${OpenCV_INCLUDE_DIRS})
add_library(v4l2sink v4l2sink.cpp)

add_executable(v4l2sinktest v4l2sinktest.cpp)
target_link_libraries(v4l2sinktest v4l2sink ${OpenCV_LIBRARIES})

add_executable(simpleopen simpleopen.cpp)
target_link_libraries(simpleopen  ${OpenCV_LIBRARIES})