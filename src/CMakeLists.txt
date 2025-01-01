#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.15)

set(LIST_HEADER_LIST
    "${list_SOURCE_DIR}/include/clist/clist.h"
)

add_library(clist
    clist.c
    rand.c
    rand.h
    ${LIST_HEADER_LIST}
)

target_include_directories(clist PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_executable(TEST-clist
    TEST-clist.c
)

target_link_libraries(TEST-clist clist)

add_test(
    NAME clist
    COMMAND $<TARGET_FILE:TEST-clist>
)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${LIST_HEADER_LIST}
)

