# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

if (NOT DEFINED TULPAR_AUDIO_NAME)
    set(TULPAR_AUDIO_NAME "TulparAudio" CACHE STRING "Project name for Tulpar Audio library")
endif()

if (NOT DEFINED INSTALL_INCLUDE_DIR)
    set(INSTALL_INCLUDE_DIR "include/godlike" CACHE STRING "Path to directory holding headers")
endif()

if (NOT DEFINED INSTALL_LIBRARY_DIR)
    set(INSTALL_LIBRARY_DIR "lib" CACHE STRING "Path to directory holding libraries")
endif()

if (NOT DEFINED INSTALL_DATA_DIR)
    set(INSTALL_DATA_DIR "share" CACHE STRING "Path to directory holding additional data")
endif()

set(TULPAR_AUDIO_LIB ${TULPAR_AUDIO_NAME} CACHE STRING "Name of Tulpar Audio library")
set(TULPAR_AUDIO_LIB_FULL ${TULPAR_AUDIO_LIB}.dll CACHE STRING "Full name of Tulpar Audio library")

set(TULPAR_INSTALL_INCLUDE_DIR ${INSTALL_INCLUDE_DIR})
set(TULPAR_INSTALL_LIBRARY_DIR ${INSTALL_LIBRARY_DIR}/${TULPAR_AUDIO_NAME})
set(TULPAR_INSTALL_DATA_DIR ${INSTALL_DATA_DIR}/${TULPAR_AUDIO_NAME})

set(TULPAR_VENDOR "Godlike")
set(TULPAR_DESCRIPTION "Tulpar Audio library")
set(TULPAR_COMMENT "")
set(TULPAR_COPYRIGHT "Copyright (C) 2017 by Godlike")
set(TULPAR_LEGAL_TM "Distributed under the MIT license (http://opensource.org/licenses/MIT)")

set(TULPAR_VERSION_MAJOR 0)
set(TULPAR_VERSION_MINOR 1)
set(TULPAR_VERSION_PATCH 0)
set(TULPAR_VERSION_TWEAK 0)

set(TULPAR_VERSION "${TULPAR_VERSION_MAJOR}.${TULPAR_VERSION_MINOR}.${TULPAR_VERSION_PATCH}")
set(TULPAR_SOVERSION "${TULPAR_VERSION_MAJOR}.${TULPAR_VERSION_MINOR}")

if (NOT DEFINED SLEIPNIR_VERSION_MAJOR)
    set(SLEIPNIR_VERSION_MAJOR 0)
endif()

if (NOT DEFINED SLEIPNIR_VERSION_MINOR)
    set(SLEIPNIR_VERSION_MINOR 0)
endif()

if (NOT DEFINED SLEIPNIR_VERSION_PATCH)
    set(SLEIPNIR_VERSION_PATCH 0)
endif()

if (NOT DEFINED SLEIPNIR_VERSION_TWEAK)
    set(SLEIPNIR_VERSION_TWEAK 0)
endif()
