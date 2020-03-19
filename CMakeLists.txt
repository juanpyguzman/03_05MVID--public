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

set(MATHLIB_SRC_FILES
  ${CMAKE_CURRENT_LIST_DIR}/src/mathlib/mat3.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/mathlib/mat4.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/mathlib/vec2.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/mathlib/vec3.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/mathlib/vec4.cc)

set(MATHLIB_HEADER_FILES
  ${CMAKE_CURRENT_LIST_DIR}/include/mathlib/defines.h
  ${CMAKE_CURRENT_LIST_DIR}/include/mathlib/mat3.h
  ${CMAKE_CURRENT_LIST_DIR}/include/mathlib/mat4.h
  ${CMAKE_CURRENT_LIST_DIR}/include/mathlib/vec2.h
  ${CMAKE_CURRENT_LIST_DIR}/include/mathlib/vec3.h
  ${CMAKE_CURRENT_LIST_DIR}/include/mathlib/vec4.h)

set(ENGINE_SRC_FILES
  ${CMAKE_CURRENT_LIST_DIR}/src/engine/debug_draw.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/engine/game.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/engine/sprite.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/engine/texture.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/engine/window.cc)

set(ENGINE_HEADER_FILES
  ${CMAKE_CURRENT_LIST_DIR}/include/engine/debug_draw.h
  ${CMAKE_CURRENT_LIST_DIR}/include/engine/game.h
  ${CMAKE_CURRENT_LIST_DIR}/include/engine/math.h
  ${CMAKE_CURRENT_LIST_DIR}/include/engine/scene.h
  ${CMAKE_CURRENT_LIST_DIR}/include/engine/texture.h
  ${CMAKE_CURRENT_LIST_DIR}/include/engine/window.h)

set(IA_SRC_FILES
  ${CMAKE_CURRENT_LIST_DIR}/src/ia/agent.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/ia/body.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/ia/mind.cc
  ${CMAKE_CURRENT_LIST_DIR}/src/ia/scene_steering.cc)

set(IA_HEADER_FILES
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/agent.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/body.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/defines.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/map.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/mind.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/scene_steering.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/world.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/kinematic/kinematicarrive.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/kinematic/kinematicflee.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/kinematic/kinematicseek.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/kinematic/kinematicwander.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/steering/delegated/face.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/steering/delegated/lookgoing.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/steering/delegated/pursue.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/steering/delegated/wander.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/steering/align.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/steering/arrive.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/steering/flee.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/steering/seek.h
  ${CMAKE_CURRENT_LIST_DIR}/include/ia/movement/steering/velocity_matching.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PREFIX "Source Files"
  FILES ${NET_SRC_FILES} ${MATHLIB_SRC_FILES} ${ENGINE_SRC_FILES} ${IA_SRC_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
  PREFIX "Header Files"
  FILES ${CONTAINERS_HEADER_FILES} ${NET_HEADER_FILES} ${MATHLIB_HEADER_FILES} ${ENGINE_HEADER_FILES} ${IA_HEADER_FILES})

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

#****************************************************
# AG03 - Steering Behaviors, ACT03 - Pathfinding, ACT04 - FinalCastle, OPT01 - Flocking
#****************************************************
function(project_for_ia TARGET_NAME)
  add_executable(${TARGET_NAME}
      ${PROJECTS_PATH}/${TARGET_NAME}/main.cpp
      ${MATHLIB_SRC_FILES} ${MATHLIB_HEADER_FILES}
      ${ENGINE_SRC_FILES} ${ENGINE_HEADER_FILES}
      ${IA_SRC_FILES} ${IA_HEADER_FILES})
  target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include)
  target_compile_options(${TARGET_NAME} PRIVATE "/W4")
  target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
  target_link_libraries(${TARGET_NAME} PRIVATE
    debug ${CMAKE_CURRENT_LIST_DIR}/libs/Debug/SDL2.lib
    debug ${CMAKE_CURRENT_LIST_DIR}/libs/Debug/SDL2main.lib
    debug ${CMAKE_CURRENT_LIST_DIR}/libs/Debug/SDL2_ttf.lib
    debug ${CMAKE_CURRENT_LIST_DIR}/libs/Debug/SDL2_image.lib
    optimized ${CMAKE_CURRENT_LIST_DIR}/libs/Release/SDL2.lib
    optimized ${CMAKE_CURRENT_LIST_DIR}/libs/Release/SDL2main.lib
    optimized ${CMAKE_CURRENT_LIST_DIR}/libs/Release/SDL2_ttf.lib
    optimized ${CMAKE_CURRENT_LIST_DIR}/libs/Release/SDL2_image.lib)

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_LIST_DIR}/libs/$<CONFIGURATION>/libfreetype-6.dll"
    "${CMAKE_CURRENT_LIST_DIR}/libs/$<CONFIGURATION>/libjpeg-9.dll"
    "${CMAKE_CURRENT_LIST_DIR}/libs/$<CONFIGURATION>/libpng16-16.dll"
    "${CMAKE_CURRENT_LIST_DIR}/libs/$<CONFIGURATION>/libtiff-5.dll"
    "${CMAKE_CURRENT_LIST_DIR}/libs/$<CONFIGURATION>/libwebp-7.dll"
    "${CMAKE_CURRENT_LIST_DIR}/libs/$<CONFIGURATION>/SDL2.dll"
    "${CMAKE_CURRENT_LIST_DIR}/libs/$<CONFIGURATION>/SDL2_image.dll"
    "${CMAKE_CURRENT_LIST_DIR}/libs/$<CONFIGURATION>/SDL2_ttf.dll"
    "${CMAKE_CURRENT_LIST_DIR}/libs/$<CONFIGURATION>/zlib1.dll"
    $<TARGET_FILE_DIR:${TARGET_NAME}>)
endfunction()

set(IA_PROJECTS AG03 ACT03 ACT04 OPT01)
foreach(PROJECT ${IA_PROJECTS})
  project_for_ia(${PROJECT})
endforeach()