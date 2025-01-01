//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <assert.h>
#include <list/list.h>
#include <stdio.h>

void test_push_get(void) {
    CDLinkedList list = newCDLinkedList(sizeof(int));
    int num = 123;
    list.pushFront(&list, &num);
    assert(list.length == 1);
    num = 456;
    list.pushFront(&list, &num);
    assert(list.length == 2);
    num = 789;
    list.pushFront(&list, &num);
    assert(list.length == 3);
    num = *(int *)list.get(&list, 0);
    assert(num = 789);
    num = *(int *)list.get(&list, 0);
    assert(num = 456);
    num = *(int *)list.get(&list, 0);
    assert(num = 123);
    list.free(&list);
    assert(list.length == 0);
    assert(list.first == NULL);
}

void test_remove(void) {
    CDLinkedList list = newCDLinkedList(sizeof(int));
    int num = 123;
    list.pushFront(&list, &num);
    assert(list.length == 1);
    num = 456;
    list.pushFront(&list, &num);
    assert(list.length == 2);
    num = 789;
    list.pushFront(&list, &num);
    assert(list.length == 3);
    num = 1234;
    list.pushFront(&list, &num);
    assert(list.length == 4);

    list.remove(&list, 1);

    num = *(int *)list.get(&list, 0);
    assert(num = 1234);
    num = *(int *)list.get(&list, 0);
    assert(num = 456);
    num = *(int *)list.get(&list, 0);
    assert(num = 123);
    list.free(&list);
    assert(list.length == 0);
    assert(list.first == NULL);
}

void print_int(void *intp) {
    int val = *(int *)intp;
    printf("%d ", val);
}

void test_shuffle_iter(void) {
    CDLinkedList list = newCDLinkedList(sizeof(int));

    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 10; ++i) {
        list.pushFront(&list, arr + i);
    }
    int ret = list.shuffle(&list);
    assert(ret == 0);

    list.iter(&list, &print_int);

    list.free(&list);
    putchar('\n');
}

int main(void) {
    test_push_get();
    test_remove();
    test_shuffle_iter();
}

