cmake_minimum_required(VERSION 3.20)
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()
set(CMAKE_CXX_STANDARD 14)
project(synth)

# set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
# set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
# add_subdirectory(libs/glfw)

add_subdirectory(libs/portaudio)

file(GLOB_RECURSE SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/*.h")
file(GLOB_RECURSE HDRS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HDRS} libs/glad/src/glad.c)

add_library(glad libs/glad/src/glad.c)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw)
target_include_directories(glad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include)

target_link_libraries(${PROJECT_NAME} PortAudio)
if (APPLE)
  target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/libglfw.3.dylib)
endif()
target_link_libraries(${PROJECT_NAME} glad)
