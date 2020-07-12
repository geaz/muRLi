cmake_minimum_required(VERSION 3.12)
project(muRli VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 17)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-kissfft.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-rtAudio.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-zLib.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-ixWebsocket.txt)
    
file(GLOB_RECURSE MURLIDESKTOP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(muRli SHARED ${MURLIDESKTOP_SOURCES})
add_dependencies(muRli
    ext_zLib
    ext_ixws
    ext_rtaudio)
target_link_libraries(muRli
    kissfft
    zlibstatic
    ixwebsocket
    rtaudio)
target_include_directories(muRli PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/src/network/websocket/
    ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/src/network/websocket/commands)
target_compile_definitions(muRli PRIVATE BAR_COUNT=17 BAR_HEIGHT=8)

# Necessary for ixWebsocket
if(WIN32)
    target_link_libraries(muRli ws2_32)
endif()