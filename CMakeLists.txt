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
# AG01 - Multithreading
#****************************************************
add_executable(AG01
    ${PROJECTS_PATH}/AG01/main.cpp)
configure_compiler(AG01)
target_compile_features(AG01 PRIVATE cxx_std_17)

#****************************************************
# ACT01 - Multithreading
#****************************************************
add_executable(ACT01
    ${PROJECTS_PATH}/ACT01/main.cpp
    ${CONTAINERS_HEADER_FILES})
target_include_directories(ACT01 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
configure_compiler(ACT01)
target_compile_features(ACT01 PRIVATE cxx_std_17)