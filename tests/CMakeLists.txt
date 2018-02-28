# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)

list(APPEND CMAKE_MODULE_PATH "${CATCH_ROOT}/contrib")

include(ParseAndAddCatchTests)

include_directories(${TULPAR_INCLUDE_DIR} ${CATCH_ROOT}/single_include)

set(TARGET_FOLDER_ROOT "${TARGET_FOLDER_ROOT}/tests")

add_subdirectory(internal)
