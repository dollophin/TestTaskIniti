find_package(GTest REQUIRED)

add_executable(UnitTests
   "TestTaskInitiTestsMain.cpp"
   "TestTaskInitiTestsInitialization.cpp"
   "TestTaskInitiTestsSerialization.cpp"
)

target_link_libraries(UnitTests
    GTest::gtest
    GTest::gtest_main
    TestTaskCore
)

target_include_directories(UnitTests PRIVATE ${CMAKE_SOURCE_DIR}/src)

include(GoogleTest)
gtest_discover_tests(UnitTests)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()
