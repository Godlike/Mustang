# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

if (NOT DEFINED TULPAR_AUDIO_NAME)
    set(TULPAR_AUDIO_NAME "TulparAudio" CACHE STRING "Project name for Tulpar Audio library")
endif()

project(${TULPAR_AUDIO_NAME})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif (NOT CMAKE_BUILD_TYPE)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /MP /W3") # TODO set WX (warning as error)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include(GlmConfig)
find_package(GLM)

if (NOT GLM_FOUND)
    add_subdirectory(${GLM_ROOT_DIR})
endif()

## Installation info
include(InstallInfo)

include(stb)

find_package(OpenAL REQUIRED)

include_directories(
    ${TULPAR_AUDIO_INCLUDE_DIRS}
)

include_directories(SYSTEM
    ${GLM_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
)

set(TULPAR_AUDIO_HEADERS
    include/tulpar/OpenALContext.hpp
    include/tulpar/AudioSource.hpp)

set(TULPAR_AUDIO_SOURCES
    source/OpenALContext.cpp
    source/AudioSource.cpp)

add_library(${TULPAR_AUDIO_LIB}
    ${TULPAR_AUDIO_HEADERS}
    ${TULPAR_AUDIO_SOURCES}
)

target_link_libraries(${TULPAR_AUDIO_LIB}
    ${OPENAL_LIBRARY}
)

add_custom_command(TARGET ${TULPAR_AUDIO_LIB} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                        "${TULPAR_AUDIO_ROOT}/data/"
                        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/")
