# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

project(BasicTulparDemo)

set(CMAKE_CXX_STANDARD 14)

set(AdditionalCatchParameters
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
)

include_directories(
    ${TULPAR_INCLUDE_DIR}
)

include_directories( SYSTEM
    ${MULE_INCLUDE_DIR}
    ${TULPAR_EXTERNAL_INCLUDE_DIRS}
)

add_definitions(
    -DTULPAR_DEMO_WORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${TULPAR_AUDIO_LIB})

set_target_properties(
    ${PROJECT_NAME}

    PROPERTIES

    FOLDER "${TARGET_FOLDER_ROOT}"
)

