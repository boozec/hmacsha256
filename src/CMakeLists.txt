include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES "*.cc")

add_library(sha256 sha256.cc)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
