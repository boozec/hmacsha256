cmake_minimum_required(VERSION 2.8.9)
project(hmacsha256)

include_directories(include)

file(GLOB SOURCES "src/*.cc")

add_executable(hmacsha256 ${SOURCES})
