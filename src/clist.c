//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rand.h"
#include <clist/clist.h>



typedef struct pointer_arr {
    size_t current_index;
    void **arr;
} PointerArr;

/* public methods */
void *cdll_get(CDLinkedList *list, size_t index);
void *cdll_insert_front(CDLinkedList *list,void *data);
void *cdll_insert_back(CDLinkedList *list, void *data);
void cdll_remove(CDLinkedList *list, size_t index);
void cdll_sort(CDLinkedList *list, int (*cmp)(void *thing1, void *thing2));
int cdll_shuffle(CDLinkedList *list);
void cdll_iter(CDLinkedList *list, void (*func)(void *data));
void cdll_free(CDLinkedList *list);


// this is only here because Andy requires it
CDLinkedListNode *makeNode(void *data, size_t data_size);

/**
 * `data` is passed unmodified into `func` as `user_data`
 * this gives the user a bit extra state to play with
 * it also avoids static variables which are evil
 *
 * list_data is ptr to ptr so that the user can modify where the pointer stored in the list points.
 * if you decide to modify *list_data, you must ensure that the original value of *list_data remains in the linked list somewhere to avoid memory leaks!
 */
void cdll_iter_extra_state(CDLinkedList *list, void *data, void (*func)(void **list_data, void *user_data));



const cdll_vtable main_cdll_vtable = {
    .get = &cdll_get,
    .insertFront = &cdll_insert_front,
    .insertBack = &cdll_insert_back,
    .remove = &cdll_remove,
    .sort = &cdll_sort,
    .shuffle = &cdll_shuffle,
    .iter = &cdll_iter,
    .iter2 = &cdll_iter_extra_state,
    .free = &cdll_free,
};


/* private methods */
CDLinkedListNode *cdll_get_internal(CDLinkedList *list, size_t index);

/**
 * Swaps `node` with the next one in the list
 * Only safe to call when there are at least two nodes in the list!
 * Does not modify the pointer structure of the list at all, just swaps around the data pointers.
 */
void cdll_swap_nodes(CDLinkedListNode *node); // swaps the node passed as argument with the next one in the list
void list_into_arr_internal(void **list_data, void *arr);
void arr_into_list_internal(void **list_data, void *arr);

/* constructor */
CDLinkedList newCDLinkedList(size_t data_size) {
    CDLinkedList newlinklist = {
        .first = NULL,
        .length = 0,
        .data_size = data_size,
        .f = &main_cdll_vtable,
    };
    return newlinklist;
}


CDLinkedListNode *makeNode(void *data, size_t data_size) {
    CDLinkedListNode *node = malloc(sizeof(CDLinkedListNode));
    if (node == NULL) {
        return NULL;
    }

    node->next = NULL;
    node->prev = NULL;
    node->data = malloc(data_size);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }

    memcpy(node->data, data, data_size);
    return node;
}




void *cdll_get(CDLinkedList *list, size_t index) {
    return cdll_get_internal(list, index)->data;
}


void *cdll_insert_front(CDLinkedList *list, void *data) {
    void *dat = NULL;
    if ((dat = cdll_insert_back(list, data)) == NULL) {
        return NULL;
    }
    if (list->length < 2) {
        return dat;
    }
    list->first = list->first->prev;
    return dat;
}

void *cdll_insert_back(CDLinkedList *list, void *data) {
    CDLinkedListNode *next_front = makeNode(data, list->data_size);
    if (next_front == NULL) {
        return NULL;
    }
    ++(list->length);
    if (list->length == 1) {
        next_front->next = NULL;
        next_front->prev = NULL;
        list->first = next_front;
        return list->first->data;
    }

    if (list->length == 2) {
        list->first->next = next_front;
        list->first->prev = next_front;
        next_front->next = list->first;
        next_front->prev = list->first;
        return list->first->data;
    }

    next_front->next = list->first;
    next_front->prev = list->first->prev;
    
    list->first->prev->next = next_front;
    list->first->prev = next_front;

    return next_front->data;
}

void cdll_remove(CDLinkedList *list, size_t index) {
    CDLinkedListNode *target = cdll_get_internal(list, index);
    if (target == NULL) {
        return;
    }
    --(list->length);
    if (list->length == 0) {
        free(target->data);
        free(target);
        list->first = NULL;
        return;
    }
    if (list->length == 1) {
        target->next->next = NULL;
        target->next->prev = NULL;
        free(target->data);
        free(target);
        return;
    }

    target->prev->next = target->next;
    target->next->prev = target->prev;
    free(target->data);
    free(target);
}


void cdll_sort(CDLinkedList *list, int (*cmp)(void *thing1, void *thing2)) {
    CDLinkedListNode *current;
    bool has_swapped = true;
    if (list->length < 2) {
        return;
    }

    for (size_t end = list->length - 1; end > 0 && has_swapped; --end) {
        has_swapped = false;
        current = list->first;
        for (size_t i = 0; i < end; ++i) {
            if (cmp(current->data, current->next->data) > 0) {
                has_swapped = true;
                cdll_swap_nodes(current);
            }
            current = current->next; // `cdll_swap_nodes` does not modify the pointer structure at all so this is valid
        }
    }
}



int cdll_shuffle(CDLinkedList *list) {
    PointerArr parr = {.current_index = 0}; // wrapper struct for an array of void *
    int rand_num;
    void *temp;
    parr.arr = malloc(sizeof(void *) * list->length);
    if (parr.arr == NULL) {
        return -1;
    }
    
    /*
    Copying the list into an array is done since random access on a linked list is O(n), with n random accesses it's O(n^2).
    Copying into an array is O(n), each random array access is O(1), with n random accesses it's total O(n + n) = O(n) which is better than O(n^2)

    summary: linked lists are dumb in most cases
    */
    list->f->iter2(list, &parr, &list_into_arr_internal);
    
    for (int i = 0; i < list->length - 1; ++i) {
        rand_num = randint(i, list->length);
        temp = parr.arr[i];
        parr.arr[i] = parr.arr[rand_num];
        parr.arr[rand_num] = temp;
    }
    
    parr.current_index = 0;
    list->f->iter2(list, &parr, &arr_into_list_internal);
    
    free(parr.arr);
    return 0;
}


void cdll_iter(CDLinkedList *list, void (*func)(void *data)) {
    CDLinkedListNode *current = list->first;
    if (current == NULL) {
        return;
    }

    for (size_t i = 0; i < list->length; ++i) {
        func(current->data);
        current = current->next;
    }
}


void cdll_iter_extra_state(CDLinkedList *list, void *data, void (*func)(void **list_data, void *user_data)) {
    CDLinkedListNode *current = list->first;
    if (current == NULL) {
        return;
    }

    for (size_t i = 0; i < list->length; ++i) {
        func(&(current->data), data);
        current = current->next;
    }
}

CDLinkedListNode *cdll_get_internal(CDLinkedList *list, size_t index) {
    CDLinkedListNode *current = list->first;
    if (current == NULL) {
        return NULL;
    }
    index = index % list->length; // since the list is circular, you'd loop back around anyways
                                  // conveniently this also takes care of the special case, length=1, since the for loop gets skipped
    for (int i = 0; i < index; ++i) { // could technically loop from back to front in cases where it would be faster but oh well
        current = current->next;
    }

    return current;
}

void cdll_free(CDLinkedList *list) {
    CDLinkedListNode *current_node;
    for (size_t i = 0; i < list->length; ++i) {
        current_node = list->first;
        list->first = current_node->next; // technically the last node in the list will have a dangling pointer but it's all getting freed anyways so we don't care
        free(current_node->data);
        free(current_node);
    }
    list->first = NULL;
    list->length = 0;
}

void cdll_swap_nodes(CDLinkedListNode *node) {
    void *temp = node->data;
    node->data = node->next->data;
    node->next->data = temp;
}

void list_into_arr_internal(void **list_data, void *arr) {
    PointerArr *parr = (PointerArr *)arr;
    parr->arr[parr->current_index++] = *list_data;
}

void arr_into_list_internal(void **list_data, void *arr) {
    PointerArr *parr = (PointerArr *)arr;
    *list_data = parr->arr[parr->current_index++];
}

