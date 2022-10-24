#Quickstart CMake taken from:
# https://google.github.io/googletest/quickstart-cmake.html
set(CMAKE_CXX_STANDARD 17)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(
    unit_testing-classes
    unit_testing-classes.cc
    ${DotsAndBoxes_SOURCE_DIR}/src/Cell.cc
    ${DotsAndBoxes_SOURCE_DIR}/src/Board.cc
    ${DotsAndBoxes_SOURCE_DIR}/src/Movement.cc
    ${DotsAndBoxes_SOURCE_DIR}/src/Player.cc
    ${DotsAndBoxes_SOURCE_DIR}/src/MiniMax.cc
)

target_include_directories(unit_testing-classes PUBLIC "${DotsAndBoxes_SOURCE_DIR}/include")
    target_link_libraries(
    unit_testing-classes
    GTest::gtest_main
)
include(GoogleTest)

gtest_discover_tests(unit_testing-classes)