# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(Tulpar)

find_package(OpenAL REQUIRED)

include_directories(${OPENAL_INCLUDE_DIR})

add_executable (tulpar Main.cpp)

target_link_libraries(tulpar ${OPENAL_LIBRARY})
