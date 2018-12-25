# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

project(BasicTulparDemo)

add_executable(${PROJECT_NAME} "")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

add_definitions(
    -DTULPAR_DEMO_WORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        main.cpp
)

target_link_libraries(${PROJECT_NAME}
    Tulpar::Audio
)

set_target_properties(
    ${PROJECT_NAME}

    PROPERTIES

    FOLDER "${TARGET_FOLDER_ROOT}"
)

