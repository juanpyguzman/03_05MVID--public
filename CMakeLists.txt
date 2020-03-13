cmake_minimum_required(VERSION 3.10)

project(05MVID CXX C)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Config types" FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Root path of the projects
set(PROJECTS_PATH ${CMAKE_CURRENT_LIST_DIR}/projects)

set(CONTAINERS_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/include/containers/vector.hpp)

set(NET_SRC_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/net/tcpsocket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/udpsocket.cpp)

set(NET_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/include/net/socketaddress.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/socketutils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/tcpsocket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/udpsocket.h)

#****************************************************
# AG01 - Multithreading
#****************************************************
add_executable(AG01
    ${PROJECTS_PATH}/AG01/main.cpp)
target_compile_options(AG01 PRIVATE "/W4")
target_compile_features(AG01 PRIVATE cxx_std_17)

#****************************************************
# ACT01 - Multithreading
#****************************************************
add_executable(ACT01
    ${PROJECTS_PATH}/ACT01/main.cpp
    ${CONTAINERS_HEADER_FILES})
target_include_directories(ACT01 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_options(ACT01 PRIVATE "/W4")
target_compile_features(ACT01 PRIVATE cxx_std_17)

#****************************************************
# AG02 - Multiplayer, ACT02 - Multiplayer, Rock, Paper, Scissors, Lizard, Spock
#****************************************************
function(project_for_net TARGET_NAME)
  add_executable(${TARGET_NAME}
      ${PROJECTS_PATH}/${TARGET_NAME}/main.cpp
      ${NET_SRC_FILES} ${NET_HEADER_FILES})
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
  target_compile_options(${TARGET_NAME} PRIVATE "/W4")
  target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
  target_link_libraries(${TARGET_NAME} PRIVATE Ws2_32)
endfunction()

set(NET_PROJECTS AG02_UDP_Client AG02_UDP_Server AG02_TCP_Client AG02_TCP_Server ACT02_Client ACT02_Server)
foreach(PROJECT ${NET_PROJECTS})
  project_for_net(${PROJECT})
endforeach()

