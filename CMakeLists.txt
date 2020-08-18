cmake_minimum_required(VERSION 2.8)

PROJECT (Cluster)

find_package(OpenCV REQUIRED )

set( SOURCE
    src/main.cpp    
)

set( HEADERS       
     include/header.h
)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
link_directories( ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_executable( cluster ${SOURCE} ${HEADERS} )

target_link_libraries( cluster ${OpenCV_LIBS} )