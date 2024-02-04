cmake_minimum_required(VERSION 3.27)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME} PRIVATE sierpinski.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE graphics)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${SIERPINSKI_BIN_DIR}")
