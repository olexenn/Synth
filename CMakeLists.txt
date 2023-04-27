cmake_minimum_required(VERSION 3.20)
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()
set(CMAKE_CXX_STANDARD 17)
project(dudo-synth)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(libs/portaudio)
add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)

file(GLOB_RECURSE SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/*.h")
file(GLOB_RECURSE HDRS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HDRS})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/presets $<TARGET_FILE_DIR:${PROJECT_NAME}>/presets)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)

target_link_libraries(${PROJECT_NAME} PUBLIC PortAudio glfw glad)

add_subdirectory(tests)

# if (NOT googletest)
#   include(FetchContent)
#   FetchContent_Declare(
#     googletest
#     URL https://github.com/google/googletest/archive/refs/heads/master.zip
#   )

#   set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#   FetchContent_MakeAvailable(googletest)
# endif()
