# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

project(${TULPAR_AUDIO_NAME})

set(TULPAR_LIB_ALIAS Tulpar::Audio)

add_library(${PROJECT_NAME} "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(${TULPAR_LIB_ALIAS} ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

find_package(Threads)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (UNIX)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-command-line-argument")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb3 -O0")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W3") # TODO set WX (warning as error)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DAL_LIBTYPE_STATIC)
endif()

## Installation info
include(TulparInstallInfo)

# OpenAL
include(OpenALConfig)

# Force LIBTYPE to be STATIC while saving initial value to temp variable
set(TMP_LIBTYPE ${LIBTYPE})
set(LIBTYPE "STATIC")

add_subdirectory(${OPENAL_ROOT_DIR})

set(LIBTYPE ${TMP_LIBTYPE})

message(STATUS "Version: ${TULPAR_VERSION}")

if (NOT GLM_FOUND)
    add_subdirectory(${GLM_ROOT_DIR})
endif()

if (NOT MULE_FOUND)
    include(MuleConfig)
    add_subdirectory("${MULE_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/Mule")
endif()

include_directories(
    ${TULPAR_INCLUDE_DIR}
    ${TULPAR_INTERNAL_INCLUDE_DIR}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${SLEIPNIR_INSTALL_INCLUDE_DIR}>
        $<TARGET_PROPERTY:Tulpar::Audio::Audio,INTERFACE_INCLUDE_DIRECTORIES>

    SYSTEM PUBLIC
        ${TULPAR_EXTERNAL_INCLUDE_DIRS}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${TULPAR_INSTALL_INCLUDE_DIR}
    COMPONENT tulpar_headers
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.imp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "InternalLoggers.hpp" EXCLUDE
)

set(TARGET_FOLDER_ROOT "${TARGET_FOLDER_ROOT}/audio")

add_subdirectory(audio)
add_subdirectory(internal)

set(TULPAR_AUDIO_HEADERS
    include/tulpar/InternalLoggers.hpp
    include/tulpar/Loggers.hpp

    include/tulpar/TulparAudio.hpp
    include/tulpar/TulparConfigurator.hpp
)

set(TULPAR_AUDIO_SOURCES
    source/Loggers.cpp

    source/TulparAudio.cpp
    source/TulparConfigurator.cpp
)

set(TULPAR_AUDIO_EXTRA)

if (MSVC)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/TulparVersionInfo.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/TulparVersionInfo.rc

        @ONLY
    )

    set(TULPAR_AUDIO_EXTRA
        ${TULPAR_AUDIO_EXTRA}
        ${CMAKE_CURRENT_BINARY_DIR}/TulparVersionInfo.rc
    )
endif()

target_sources(${PROJECT_NAME}
    PRIVATE
        ${TULPAR_AUDIO_HEADERS}
        ${TULPAR_AUDIO_SOURCES}

        ${TULPAR_AUDIO_EXTRA}

        $<TARGET_OBJECTS:Tulpar::Audio::Internal>
        $<TARGET_OBJECTS:Tulpar::Audio::Audio>
)

# external dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Mule::Utilities
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${OPENAL_LIBRARY}
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        VERSION ${TULPAR_VERSION}
        SOVERSION ${TULPAR_SOVERSION}
)

install( TARGETS ${PROJECT_NAME}
    COMPONENT tulpar_libraries
    RUNTIME DESTINATION ${TULPAR_INSTALL_LIBRARY_DIR}
    LIBRARY DESTINATION ${TULPAR_INSTALL_LIBRARY_DIR}
    ARCHIVE DESTINATION ${TULPAR_INSTALL_LIBRARY_DIR}
)

install( DIRECTORY ${OPENAL_INCLUDE_DIR}/AL
    DESTINATION ${TULPAR_INSTALL_INCLUDE_DIR}
    COMPONENT tulpar_extra
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.h"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.inl"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

if (TULPAR_BUILD_DOCUMENTATION)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        set(DOXYGEN_INPUT_DIRS
            ${TULPAR_INCLUDE_DIR}
        )
        set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
        string(REPLACE ";" " " DOXYGEN_INPUT "${DOXYGEN_INPUT_DIRS}")

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
        add_custom_target( docs
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
            COMMENT "Generating Tulpar Audio API documentation"
        )
    else()
        message(FATAL_ERROR "Doxygen is required to build documentation")
    endif()
endif()


include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${TULPAR_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake.gen

    @ONLY
)

file(GENERATE
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake.gen
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${TULPAR_INSTALL_LIBRARY_DIR}
    COMPONENT tulpar_headers
)

install( DIRECTORY ${GLM_INCLUDE_DIR}/glm
    DESTINATION ${TULPAR_INSTALL_INCLUDE_DIR}
    COMPONENT tulpar_extra
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.h"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.inl"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)
