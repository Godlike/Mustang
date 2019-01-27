# Copyright (C) 2018 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.4)
cmake_policy(VERSION 3.4)

project(CollectionTest)

set(CMAKE_CXX_STANDARD 14)

set(AdditionalCatchParameters
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
)

set(TARGET_FOLDER_ROOT "${TARGET_FOLDER_ROOT}/internal")

set(TEST_UTILS
    CollectionTestUtils.hpp
)

add_executable(BufferCollectionTest BufferCollectionTest.cpp ${TEST_UTILS})
target_link_libraries(BufferCollectionTest Tulpar::Audio)
ParseAndAddCatchTests(BufferCollectionTest)

add_executable(SourceCollectionTest SourceCollectionTest.cpp ${TEST_UTILS})
target_link_libraries(SourceCollectionTest Tulpar::Audio)
ParseAndAddCatchTests(SourceCollectionTest)

set_target_properties(
    BufferCollectionTest
    SourceCollectionTest

    PROPERTIES

    FOLDER "${TARGET_FOLDER_ROOT}"
)

