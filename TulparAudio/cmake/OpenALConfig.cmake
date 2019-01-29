# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

set(ALSOFT_UTILS OFF CACHE BOOL "Flag to override default ALSOFT_UTILS value")
set(ALSOFT_NO_CONFIG_UTIL ON CACHE BOOL "Flag to override default ALSOFT_NO_CONFIG_UTIL value")
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Flag to override default ALSOFT_EXAMPLES value")
set(ALSOFT_TESTS OFF CACHE BOOL "Flag to override default ALSOFT_TESTS value")
set(ALSOFT_CONFIG OFF CACHE BOOL "Flag to override default ALSOFT_CONFIG value")
set(ALSOFT_HRTF_DEFS OFF CACHE BOOL "Flag to override default ALSOFT_HRTF_DEFS value")
set(ALSOFT_AMBDEC_PRESETS OFF CACHE BOOL "Flag to override default ALSOFT_AMBDEC_PRESETS value")
set(ALSOFT_INSTALL OFF CACHE BOOL "Flag to override default ALSOFT_INSTALL value")
set(LIBTYPE "STATIC" CACHE STRING "Build OpenAL as a static library")
set(OPENAL_DEFINITIONS "AL_LIBTYPE_STATIC")

if (NOT DEFINED OPENAL_ROOT_DIR)
    set(OPENAL_ROOT_DIR
        ${TULPAR_ROOT}/TulparAudio/third_party/openal-soft
        CACHE STRING "Path to OpenAL root directory")
endif()

set(OPENAL_LIBRARY "OpenAL" CACHE STRING "OpenAL library name")
set(OPENAL_INCLUDE_DIR "${OPENAL_ROOT_DIR}/include" CACHE STRING "Path to OpenAL include directory")
