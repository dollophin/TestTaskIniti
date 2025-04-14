cmake_minimum_required(VERSION 3.8)

project("TestTaskIniti")

set(CMAKE_CXX_STANDARD 17)
enable_testing()

add_custom_command(
    OUTPUT raw
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/raw.bin ${CMAKE_CURRENT_BINARY_DIR}/raw.bin
)

add_library(TestTaskCore
    "src/TestTaskInitiCoreValueWrapper.cpp"
    "src/TestTaskInitiCoreValueWrapper.h"
    "src/TestTaskInitiCoreSerializator.cpp"
    "src/TestTaskInitiCoreSerializator.h"
)

add_executable(Main
    "src/main.cpp"
    raw
)

find_program(CPPCHECK cppcheck)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

if(CPPCHECK)
    add_custom_target(
        cppcheck
        COMMAND ${CPPCHECK} --enable=all --std=c++17 --check-config --inconclusive --suppress=missingIncludeSystem  --quiet ${CMAKE_SOURCE_DIR}/src
        COMMENT "Running cppcheck"
    )
    add_dependencies(Main cppcheck)
else()
    message(WARNING "cppcheck not found")
endif()

target_link_libraries(Main PRIVATE TestTaskCore)

add_subdirectory(tests)


