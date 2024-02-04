cmake_minimum_required(VERSION 3.27)

find_package(Curses REQUIRED)

project(
  graphics
  DESCRIPTION "screen drawing controls using ncurses"
  LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE screen.cpp)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CURSES_INCLUDE_DIR}
  PUBLIC ${SIERPINSKI_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES} util)
