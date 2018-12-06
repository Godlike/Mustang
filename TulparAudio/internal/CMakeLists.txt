# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 3.8)

project(TulparAudio_Internal)

add_library(${PROJECT_NAME} STATIC "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_library(Tulpar::Audio::Internal ALIAS ${PROJECT_NAME})

# OpenAL
include(OpenALConfig)

# STB
include(stb)

set(INTERNAL_HEADERS
    include/tulpar/internal/Collection.hpp
    include/tulpar/internal/Collection.imp

    include/tulpar/internal/BufferCollection.hpp
    include/tulpar/internal/Context.hpp
    include/tulpar/internal/Device.hpp
    include/tulpar/internal/ListenerController.hpp
    include/tulpar/internal/SourceCollection.hpp
)

set(INTERNAL_SOURCES
    source/BufferCollection.cpp
    source/Context.cpp
    source/Device.cpp
    source/ListenerController.cpp
    source/SourceCollection.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${INTERNAL_HEADERS}
        ${INTERNAL_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    SYSTEM
        PUBLIC
            ${OPENAL_INCLUDE_DIR}
            ${STB_INCLUDE_DIR}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Mule::Utilities
)

set_target_properties(
    ${PROJECT_NAME}

    PROPERTIES

    FOLDER "${TARGET_FOLDER_ROOT}"
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()
