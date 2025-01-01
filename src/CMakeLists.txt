#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.15)

set(LIST_HEADER_LIST
    "${list_SOURCE_DIR}/include/list/list.h"
)

add_library(list
    list.c
    rand.c
    rand.h
    ${LIST_HEADER_LIST}
)

target_include_directories(list PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_executable(TEST-list
    TEST-list.c
)

target_link_libraries(TEST-list list)

add_test(
    NAME list
    COMMAND $<TARGET_FILE:TEST-list>
)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${LIST_HEADER_LIST}
)

