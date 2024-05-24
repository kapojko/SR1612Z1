cmake_minimum_required(VERSION 3.12)
project(SR1612Z1_Test C)

# This CMakeLists.txt is for testing purposes only.
add_compile_definitions(TESTING)

# Collect all source files
set(SR1612Z1_SOURCES
    SR1612Z1.c)
add_executable(SR1612Z1_Test ${SR1612Z1_SOURCES} SR1612Z1_Test.c)

# Setup testing
enable_testing()
include(CTest)

add_test(
    NAME SR1612Z1_UnitTest
    COMMAND SR1612Z1_Test
)
