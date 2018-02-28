# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

set(INTERNAL_HEADERS
    include/tulpar/internal/Collection.hpp
    include/tulpar/internal/Collection.imp
)

set(INTERNAL_SOURCES
    source/BufferCollection.cpp
    source/SourceCollection.cpp
)

add_library(tulpar_internal OBJECT
    ${INTERNAL_HEADERS}
    ${INTERNAL_SOURCES}
)

set_target_properties(
    tulpar_internal

    PROPERTIES

    FOLDER "${TARGET_FOLDER_ROOT}"
)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()
