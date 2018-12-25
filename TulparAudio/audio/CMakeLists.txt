# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(TulparAudio_Audio)

add_library(${PROJECT_NAME} OBJECT "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(Tulpar::Audio::Audio ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# OpenAL
include(OpenALConfig)

set(AUDIO_HEADERS
    include/tulpar/audio/Buffer.hpp
    include/tulpar/audio/Listener.hpp
    include/tulpar/audio/Source.hpp
)

set(AUDIO_SOURCES
    source/Buffer.cpp
    source/Listener.cpp
    source/Source.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${AUDIO_HEADERS}
        ${AUDIO_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    SYSTEM
        PUBLIC
            ${OPENAL_INCLUDE_DIR}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        $<TARGET_PROPERTY:Mule::Utilities,INTERFACE_INCLUDE_DIRECTORIES>
)

set_target_properties(
    ${PROJECT_NAME}

    PROPERTIES

    FOLDER "${TARGET_FOLDER_ROOT}"
)

include(TulparInstallInfo)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${TULPAR_INSTALL_INCLUDE_DIR}
    COMPONENT tulpar_headers
    FILES_MATCHING
        PATTERN "*.hpp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        PATTERN "*.imp"
            PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

if (UNIX)
    set_target_properties(
        ${PROJECT_NAME}

        PROPERTIES

        POSITION_INDEPENDENT_CODE ON
    )
endif()
