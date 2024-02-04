cmake_minimum_required(VERSION 3.27)

project(
  util
  DESCRIPTION "triangle manipulation utility functions"
  LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE triangles.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${SIERPINSKI_INCLUDE_DIR})
