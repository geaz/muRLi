cmake_minimum_required(VERSION 3.12)
project(muRLi-Desktop VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 17)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-toml11.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-rtAudio.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-zLib.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-ixWebsocket.txt)
add_dependencies(ixws external_zlib)

include_directories(
    ${toml11_INCLUDE_DIR}
    ${rtaudio_INCLUDE_DIR}
    ${ixWebsocket_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../firmware/src/network/websocket/commands)

link_directories(
    ${zLib_LIBRARY_DIR}
    ${wxWidgets_LIBRARY_DIR}
    ${rtaudio_LIBRARY_DIR}
    ${ixWebsocket_LIBRARY_DIR})
    
file(GLOB_RECURSE MURLIDESKTOP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(muRLi-Desktop ${MURLIDESKTOP_SOURCES})
add_dependencies(muRLi-Desktop external_zlib ixws toml11 rta)
target_link_libraries(muRLi-Desktop
    zlibstatic
    ixwebsocket
    rtaudio)
if(WIN32)
    target_link_libraries(muRLi-Desktop ws2_32)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.toml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.toml COPYONLY)