//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LIST_H
#define LIST_H

#include <stddef.h>

// macros were the only way to emulate C++ templates or Rust generics
// void * could work, but this is *far* more cursed and marginally more efficient
// i've also reinvented name mangling, so that's cool



// does this qualify as macro abuse?

#define MAKE_CDLL_HEADER(member_type_name)\
typedef struct CDLinkedList_##member_type_name##_node CDLinkedList_##member_type_name##_node;/* did typedef before declaring struct since it's more readable*/\
struct CDLinkedList_##member_type_name##_node {\
    CDLinkedList_##member_type_name##_node *next;\
    CDLinkedList_##member_type_name##_node *prev;\
    member_type_name *data;\
};\
typedef struct CDLinkedList_##member_type_name CDLinkedList_##member_type_name;\
struct CDLinkedList_##member_type_name {/* C is object oriented if you try hard enough*/\
    CDLinkedList_##member_type_name##_node *first;\
    size_t length;\
    void *(*get)(CDLinkedList_##member_type_name *list, size_t index);\
    void *(*pushFront)(CDLinkedList_##member_type_name *list, member_type_name *data);\
    void (*remove)(CDLinkedList_##member_type_name *list, size_t index);\
    /**
     * Sorts the list with bubble sort.
     * I'd use a better algorithm but if you're using a linked list you probably don't care about performance (cache misses have already ruined your day)
     */\
    void (*sort)(CDLinkedList_##member_type_name *list, int (*cmp)(member_type_name *thing1, member_type_name *thing2));\
    int (*shuffle)(CDLinkedList_##member_type_name *list);\
    void (*iter)(CDLinkedList_##member_type_name *list, void (*func)(member_type_name *data)); /* applies `func` to every element in `list`*/\
    /**
     * `data` is passed unmodified into `func` as the second parameter
     * this gives the user a bit of extra state to play with
     * using this approach, the iterator can be made threadsafe if the user wishes, which is impossible with static variables
     */\
    void (*iter2)(CDLinkedList_##member_type_name *list, void *data, void (*func)(member_type_name **list_data, void *user_data));\
    void (*free)(CDLinkedList_##member_type_name *list);\
};\
CDLinkedList_##member_type_name new_##CDLinkedList_##member_type_name(size_t data_size);

#endif
