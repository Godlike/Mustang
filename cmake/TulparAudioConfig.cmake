# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

set(TULPAR_AUDIO_NAME "TulparAudio" CACHE STRING "Tulpar project name.")

if(NOT DEFINED TULPAR_ROOT)
    set(TULPAR_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Tulpar root directory.")
endif()

list(APPEND CMAKE_MODULE_PATH "${TULPAR_ROOT}/TulparAudio/cmake")

set(TULPAR_INCLUDE_DIR
    ${TULPAR_ROOT}/TulparAudio/include
    ${TULPAR_ROOT}/TulparAudio/audio/include

    CACHE LIST "Tulpar include directories."
)

set(TULPAR_INTERNAL_INCLUDE_DIR
    ${TULPAR_ROOT}/TulparAudio/internal/include

    CACHE LIST "Tulpar internal include dirs."
)

# GLM
include(GlmConfig)
find_package(GLM)

# OpenAL
include(OpenALConfig)

# STB
include(stb)

set(TULPAR_EXTERNAL_INCLUDE_DIRS
    ${GLM_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}

    CACHE LIST "Tulpar external include dirs."
)

if (NOT DEFINED INSTALL_INCLUDE_DIR)
    set(INSTALL_INCLUDE_DIR "include/godlike" CACHE STRING "Path to directory holding headers")
endif()

if (NOT DEFINED INSTALL_LIBRARY_DIR)
    set(INSTALL_LIBRARY_DIR "lib" CACHE STRING "Path to directory holding libraries")
endif()

set(TULPAR_AUDIO_LIB ${TULPAR_AUDIO_NAME} CACHE STRING "Name of Tulpar Audio library")
set(TULPAR_AUDIO_LIB_FULL "${TULPAR_AUDIO_LIB}.dll" CACHE STRING "Full name of Tulpar Audio library")
