# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

project(AssetTest)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

set(AdditionalCatchParameters
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
)

set(TARGET_FOLDER_ROOT "${TARGET_FOLDER_ROOT}/internal")

include_directories(
    ${TULPAR_INCLUDE_DIR}
    ${TULPAR_INTERNAL_INCLUDE_DIR}
)

include_directories( SYSTEM
    ${MULE_INCLUDE_DIR}
    ${TULPAR_EXTERNAL_INCLUDE_DIRS}
)

add_executable(CollectionTest CollectionTest.cpp)
target_link_libraries(CollectionTest ${TULPAR_AUDIO_LIB})
ParseAndAddCatchTests(CollectionTest)

set_target_properties(
    CollectionTest

    PROPERTIES

    FOLDER "${TARGET_FOLDER_ROOT}"
)

