cmake_minimum_required(VERSION 3.12)
project(muRLi-Desktop VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 17)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-kissfft.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-toml11.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-rtAudio.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-zLib.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-ixWebsocket.txt)
    
file(GLOB_RECURSE MURLIDESKTOP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(muRLi-Desktop ${MURLIDESKTOP_SOURCES})
add_dependencies(muRLi-Desktop
    ext_zLib
    ext_ixws
    ext_toml11
    ext_rtaudio)
target_link_libraries(muRLi-Desktop
    kissfft
    zlibstatic
    ixwebsocket
    rtaudio)
target_include_directories(muRLi-Desktop PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/src/visualization/
    ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/src/network/websocket/
    ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/src/network/websocket/commands)
target_compile_definitions(muRLi-Desktop PRIVATE BAR_COUNT=17 BAR_HEIGHT=8)

# Necessary for ixWebsocket
if(WIN32)
    target_link_libraries(muRLi-Desktop ws2_32)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.toml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.toml COPYONLY)