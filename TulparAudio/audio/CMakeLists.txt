# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

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

add_library(audio OBJECT
    ${AUDIO_HEADERS}
    ${AUDIO_SOURCES}
)

set_target_properties(
    audio

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
        audio

        PROPERTIES

        POSITION_INDEPENDENT_CODE ON
    )
endif()
