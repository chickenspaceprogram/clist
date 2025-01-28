//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <assert.h>
#include <clist/clist.h>
#include <stdio.h>

void test_push_get(void) {
    CDLinkedList list;
    newCDLinkedList(&list, sizeof(int));
    int num = 123;
    list.f->insertFront(&list, &num);
    assert(list.length == 1);
    num = 456;
    list.f->insertFront(&list, &num);
    assert(list.length == 2);
    num = 789;
    list.f->insertFront(&list, &num);
    assert(list.length == 3);
    num = *(int *)list.f->get(&list, 0);
    assert(num == 789);
    num = *(int *)list.f->get(&list, 1);
    assert(num == 456);
    num = *(int *)list.f->get(&list, 2);
    assert(num == 123);
    list.f->free(&list);
    assert(list.length == 0);
    assert(list.first == NULL);
}

void test_remove(void) {
    CDLinkedList list;
    newCDLinkedList(&list, sizeof(int));
    int num = 123;
    list.f->insertFront(&list, &num);
    assert(list.length == 1);
    num = 456;
    list.f->insertFront(&list, &num);
    assert(list.length == 2);
    num = 789;
    list.f->insertFront(&list, &num);
    assert(list.length == 3);
    num = 1234;
    list.f->insertFront(&list, &num);
    assert(list.length == 4);

    list.f->remove(&list, 1);

    num = *(int *)list.f->get(&list, 0);
    assert(num == 1234);
    num = *(int *)list.f->get(&list, 1);
    assert(num == 456);
    num = *(int *)list.f->get(&list, 2);
    assert(num == 123);
    list.f->free(&list);
    assert(list.length == 0);
    assert(list.first == NULL);
}

void print_int(void *intp) {
    int val = *(int *)intp;
    printf("%d ", val);
}

int int_cmp(void *int1, void *int2) {
    int intone = *(int *)int1;
    int inttwo = *(int *)int2;
    if (intone > inttwo) {
        return 1;
    }
    if (intone < inttwo) {
        return -1;
    }
    return 0;
}

void test_shuffle_iter(void) {
    CDLinkedList list;
    newCDLinkedList(&list, sizeof(int));

    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 10; ++i) {
        list.f->insertFront(&list, arr + i);
    }
    int ret = list.f->shuffle(&list);
    assert(ret == 0);

    list.f->iter(&list, &print_int); putchar('\n');
    list.f->sort(&list, &int_cmp);
    list.f->iter(&list, &print_int); putchar('\n');
    
    list.f->free(&list);
    putchar('\n');
}

int main(void) {
    test_push_get();
    test_remove();
    test_shuffle_iter();
}

