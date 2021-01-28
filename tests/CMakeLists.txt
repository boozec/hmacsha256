include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(test_sha256 test_sha256.cc ${PROJECT_SOURCE_DIR}/src/sha256.cc)

enable_testing()
add_test(test_sha256 test_sha256)
