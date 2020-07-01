cmake_minimum_required(VERSION 3.10.0)
project(muRLi-Desktop VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../firmware/src/websocket/commands)

file(GLOB_RECURSE MURLIDESKTOP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(muRLi-Desktop ${MURLIDESKTOP_SOURCES})
target_link_libraries(muRLi-Desktop ${CONAN_LIBS})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.toml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.toml COPYONLY)