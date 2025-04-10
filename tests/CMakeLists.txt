cmake_minimum_required(VERSION 3.30)
project(ZephyrTests)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
file(GLOB_RECURSE TEST_SOURCES
        "unit/*.cpp"
        "integration/*.cpp"
)

add_executable(ZephyrTests ${TEST_SOURCES})

target_include_directories(ZephyrTests PRIVATE ../src ../lib)
target_link_libraries(ZephyrTests PRIVATE Boost::unit_test_framework ${LIBS})

add_test(NAME AllTests COMMAND ZephyrTests)