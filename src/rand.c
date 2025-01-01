//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "rand.h"

void seed_rand(void) {
	srand((unsigned int) time(NULL));
}

int randint(int min, int max) {
    int diff = max - min;
    return (rand() % diff) + min;
}
