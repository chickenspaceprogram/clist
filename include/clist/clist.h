//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LIST_H
#define LIST_H

#include <stddef.h>

// CircularDoublyLinkedListNode is extremely long and I'm not typing that, this isn't the Win32 API lol
typedef struct c_d_linked_list_node CDLinkedListNode; // did typedef before declaring struct since it's more readable

struct c_d_linked_list_node {
    CDLinkedListNode *next;
    CDLinkedListNode *prev;
    void *data;
};
typedef struct c_d_linked_list CDLinkedList;
typedef struct cdll_vtable cdll_vtable; 
// C is object oriented if you try hard enough 


typedef struct cdll_vtable {
    void *(*get)(CDLinkedList *list, size_t index);
    void *(*insertFront)(CDLinkedList *list, void *data);
    void *(*insertBack)(CDLinkedList *list, void *data);
    void (*remove)(CDLinkedList *list, size_t index);
    /**
     * Sorts the list with bubble sort.
     * I'd use a better algorithm but if you're using a linked list you probably don't care about performance (cache misses have already ruined your day)
     */
    void (*sort)(CDLinkedList *list, int (*cmp)(void *thing1, void *thing2));
    

    int (*shuffle)(CDLinkedList *list);

    // printList can be implemented by passing in an appropriate function pointer
    void (*iter)(CDLinkedList *list, void (*func)(void *data)); // applies `func` to every element in `list`
    
    /**
     * `data` is passed unmodified into `func` as the second parameter
     * this gives the user a bit of extra state to play with
     * using this approach, the iterator can be made threadsafe if the user wishes, which is impossible with static variables
     */
    void (*iter2)(CDLinkedList *list, void *data, void (*func)(void **list_data, void *user_data));
    void (*free)(CDLinkedList *list);
} cdll_vtable;


extern const cdll_vtable main_cdll_vtable;

struct c_d_linked_list {
    CDLinkedListNode *first;
    size_t length;
    size_t data_size;
    const cdll_vtable *f; // using a vtable minimizes the space each list takes up
};

CDLinkedList newCDLinkedList(size_t data_size);

#endif
