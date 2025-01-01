//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <assert.h>
#include <list/list.h>

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

int main(void) {
    test_push_get();
}

