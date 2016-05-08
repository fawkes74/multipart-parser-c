cmake_minimum_required(VERSION 3.0)

project(multipart-parser-c)

add_library(${PROJECT_NAME} STATIC
  multipart_parser.c
  multipart_parser.h
  multipart_consumer.h
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
  Qt5::Core
)
