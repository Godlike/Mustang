# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

set(TULPAR_AUDIO_NAME "TulparAudio" CACHE STRING "Project name for Tulpar Audio library")

if (NOT DEFINED TULPAR_AUDIO_ROOT)
    set(TULPAR_AUDIO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../TulparAudio" CACHE STRING "Path to Tulpar Audio root directory")
endif()

set(TULPAR_AUDIO_INCLUDE_DIRS
    "${TULPAR_AUDIO_ROOT}/include"
    CACHE LIST "List of Tulpar Audio include directories")

list(APPEND CMAKE_MODULE_PATH "${TULPAR_AUDIO_ROOT}/cmake")

include(GlmConfig)

find_package(GLM)

set(TULPAR_AUDIO_EXTERNAL_INCLUDE_DEPENDENCIES
    ${GLM_INCLUDE_DIR}
    CACHE LIST "List of Tulpar Audio external include dependencies")

if (BUILD_SHARED_LIBS)
    add_definitions(-DTULPAR_SHARED)
endif()
