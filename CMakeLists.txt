cmake_minimum_required(VERSION 3.16)

project("TestTaskIniti")

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT")
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

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/raw.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/raw.bin ${CMAKE_CURRENT_BINARY_DIR}/raw.bin
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/raw.bin
    COMMENT "Copying raw.bin to build directory"
)

add_custom_target(copy_raw ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/raw.bin
)

add_executable(Main
    "src/main.cpp"
)

add_dependencies(Main copy_raw)

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

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE  
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)

