cmake_minimum_required(VERSION 3.13.4)

project(wordsmith)
include_directories(${CMAKE_SOURCE_DIR}/src)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
add_definitions(-DPROG_NAME="${PROJECT_NAME}")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(LibZip REQUIRED)
include_directories(${LIBZIP_INCLUDE_DIRS})

find_package(LibMagic REQUIRED)
include_directories(${LIBMAGIC_INCLUDE_DIRS})


add_executable(wordsmith ${SOURCES})
target_link_libraries(wordsmith ${CURSES_LIBRARIES} ${LIBZIP_LIBRARIES} ${LIBMAGIC_LIBRARIES})

install(TARGETS wordsmith DESTINATION bin)
