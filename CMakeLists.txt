# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)

project(Tulpar)

option(TULPAR_BUILD_DOCUMENTATION "Build Tulpar documentation" OFF)
option(TULPAR_BUILD_DEMOS "Build Tulpar demos" ON)
option(TULPAR_BUILD_TESTS "Build Tulpar tests" ON)
option(BUILD_SHARED_LIBS "Flag indicating if we want to build shared libraries" ON)

message(STATUS "${PROJECT_NAME} ${CMAKE_BUILD_TYPE} configuration:")
message(STATUS "-- TULPAR_BUILD_DOCUMENTATION: ${TULPAR_BUILD_DOCUMENTATION}")
message(STATUS "-- TULPAR_BUILD_DEMOS: ${TULPAR_BUILD_DEMOS}")
message(STATUS "-- TULPAR_BUILD_TESTS: ${TULPAR_BUILD_TESTS}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(TulparAudioConfig)

if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
endif()

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(TulparAudio)

if (TULPAR_BUILD_DEMOS)
    add_subdirectory(demos)
endif()

if (TULPAR_BUILD_TESTS)
    enable_testing()
    include(CTest)
    set(CATCH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Catch2)
    add_subdirectory(tests)
endif()

get_directory_property(TULPAR_HAS_PARENT PARENT_DIRECTORY)

if (TULPAR_HAS_PARENT)
    set(TULPAR_AUDIO_LIB ${TULPAR_AUDIO_LIB} PARENT_SCOPE)
endif()
