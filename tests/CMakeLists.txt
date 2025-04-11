cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(ZephyrTests LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
file(GLOB_RECURSE TEST_SOURCES
        "unit/*.cpp"
        "integration/*.cpp"
)

add_executable(ZephyrTests ${TEST_SOURCES})

target_include_directories(ZephyrTests PRIVATE ../src ../lib)
target_link_libraries(ZephyrTests PRIVATE Boost::unit_test_framework ${LIBS})

add_test(NAME AllTests COMMAND ZephyrTests)
