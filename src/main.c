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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

void warranty(void);

int main() {
    Stack stack;
    if (stack_init(&stack) != 0) return 1;

#ifdef SYSTEM_CLEAR
    system("clear||cls");
#endif

    printf(
        "pfcc Copyright (C) 2022 Eleanor Helly\n"
        "This is free software; see the source code for copying conditions. "
        "There is\n"
        "ABSOLUTELY NO WARRANTY; not even for MERCHANTABILITY or FITNESS FOR A "
        "PARTICULAR\n"
        "PURPOSE. For details, type `warranty` and press return/enter.\n");

    while (1) {
        char *input = NULL;
        size_t len;
        unsigned char i;

        /* TODO: use scanf to allow multiple args, unknown command processing */
        if (getline(&input, &len, stdin) == -1) return 1;
        input[strcspn(input, "\r\n")] = 0; /* strip newline/carriage return */
        for (i = 0; input[i]; i++) input[i] = (char)tolower(input[i]);
        if (strcmp(input, "q") == 0) {
            return 0;
        } else if (strcmp(input, "+") == 0) {
            if (stack_add(&stack) == 1) return 1;
        } else if (strcmp(input, "-") == 0) {
            if (stack_sub(&stack) == 1) return 1;
        } else if (strcmp(input, "*") == 0) {
            if (stack_mul(&stack) == 1) return 1;
        } else if (strcmp(input, "/") == 0) {
            if (stack_div(&stack) == 1) return 1;
        } else if (strcmp(input, "pow") == 0) {
            if (stack_pow(&stack) == 1) return 1;
        } else if (strcmp(input, "!") == 0) {
            if (stack_fac(&stack) == 1) return 1;
        } else if (strcmp(input, "sort") == 0) {
            stack_sort(&stack);
        } else if (strcmp(input, "warranty") == 0) {
            warranty();
            free(input);
            continue;
        } else if (strcmp(input, "clear") == 0) {
            stack_clear(&stack);
        } else if (strcmp(input, "rev") == 0) {
            stack_rev(&stack);
        } else if (strcmp(input, "drop") == 0) {
            if (stack.len > 0) stack.len--;
        } else if (strlen(input) == 0) {
            stack_push(&stack, stack_get(&stack, stack.len - 1));
        } else {
            stack_push(&stack, atof(input));
        }
        free(input);

#ifdef SYSTEM_CLEAR
        system("clear||cls");
#endif
        for (i = 0; i < stack.len; i++) {
            printf("%d: %f\n", i, stack_get(&stack, i));
        }
    }
}

void warranty() {
    printf(
        "THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n"
        "APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE "
        "COPYRIGHT\n"
        "HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT "
        "WARRANTY\n"
        "OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED "
        "TO,\n");
    printf(
        "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A "
        "PARTICULAR\n"
        "PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE "
        "PROGRAM\n"
        "IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST "
        "OF\n"
        "ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n");
}
