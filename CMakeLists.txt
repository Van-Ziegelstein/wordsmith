cmake_minimum_required(VERSION 3.13.4)

project(wordsmith)
include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
add_definitions(-DPROG_NAME="${PROJECT_NAME}")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(wordsmith ${SOURCES})
target_link_libraries(wordsmith ${CURSES_LIBRARIES})

install(TARGETS wordsmith DESTINATION bin)
