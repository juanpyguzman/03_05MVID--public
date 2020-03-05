cmake_minimum_required(VERSION 3.10)

project(05MVID CXX C)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Config types" FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Root path of the projects
set(PROJECTS_PATH ${CMAKE_CURRENT_LIST_DIR}/projects)

function(configure_compiler TARGET_NAME)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${TARGET_NAME} PRIVATE "/W4")
    endif()
endfunction()

set(CONTAINERS_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/include/containers/vector.hpp)

#****************************************************
# Activity 01 - Multithreading
#****************************************************
add_executable(01_MultiThreading
    ${PROJECTS_PATH}/01_MultiThreading/main.cpp
    ${CONTAINERS_HEADER_FILES})
target_include_directories(01_MultiThreading PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
configure_compiler(01_MultiThreading)
target_compile_features(01_MultiThreading PRIVATE cxx_std_14)