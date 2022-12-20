cmake_minimum_required(VERSION 3.20)

# project(synth-test VERSION 1.0)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/master.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB SYNTH_SOURCES "${CMAKE_SOURCE_DIR}/src/audio/Envelope.cpp"
    "${CMAKE_SOURCE_DIR}/src/audio/Filter.cpp"
    "${CMAKE_SOURCE_DIR}/src/audio/Lfo.cpp"
    "${CMAKE_SOURCE_DIR}/src/audio/Oscillator.cpp")
file(GLOB SYNTH_HEADERS "${CMAKE_SOURCE_DIR}/include/audio/Envelope.h"
    "${CMAKE_SOURCE_DIR}/include/audio/Filter.h"
    "${CMAKE_SOURCE_DIR}/include/audio/Lfo.h"
    "${CMAKE_SOURCE_DIR}/include/audio/Oscillator.h")

add_executable(synth-test ${TEST_SOURCES} ${SYNTH_SOURCES} ${SYNTH_HEADERS})
target_include_directories(synth-test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(synth-test PRIVATE GTest::GTest)
