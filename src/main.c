/*
 * pfcc - interactive postfix calculator written in C
 * Copyright (C) 2022 Eleanor Helly
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

int main() {
    Stack stack;
    stack.index = -1;

    while (1) {
        char *input = NULL;
        size_t len;
        short i, stackLen;

        if (getline(&input, &len, stdin) == -1) return -1;
        input[strcspn(input, "\r\n")] = 0; /* strip newline/carriage return */
        if (strcmp(input, "q") == 0) {
            return 0;
        } else if (strcmp(input, "+") == 0) {
            if (stack_add(&stack) == -1) return -1;
        } else if (strcmp(input, "-") == 0) {
            if (stack_sub(&stack) == -1) return -1;
        } else {
            stack_push(&stack, atof(input));
        }
        free(input);

        stackLen = stack_len(&stack);
        #ifdef SYSTEM_CLEAR
        system("clear||cls");
        #endif
        for (i = 0; i < stackLen; i++) {
            printf("%d: %f\n", stackLen - i, stack.array[i]);
        }
    }
}
