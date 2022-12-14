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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef SYSTEM_CLEAR
#pragma GCC diagnostic ignored "-Wunused-result"
#define clear() system("clear||cls")
#else
#define clear()
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

void print_stack(Stack *stack);
int process(Stack *stack, char *input);
void warranty(void);

int main(int argc, char *argv[]) {
    Stack stack;
    char input[64];
    unsigned char i;

    if (stack_init(&stack)) return 1;

    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            if (process(&stack, argv[i])) return 1;
        }
        print_stack(&stack);
        return 0;
    }

    clear();
    printf(
        "pfcc Copyright (C) 2022 Eleanor Helly\n"
        "This is free software; see the source code for copying "
        "conditions. "
        "There is\n"
        "ABSOLUTELY NO WARRANTY; not even for MERCHANTABILITY or FITNESS "
        "FOR A "
        "PARTICULAR\n"
        "PURPOSE. For details, type `warranty` and press return/enter.\n");

    while (scanf("%64s", input) != EOF) {
        if (!strcmp(input, "warranty")) {
            warranty();
        } else {
            clear();
            if (process(&stack, input)) return 1;
            print_stack(&stack);
            printf("\n");
        }
    }
}

void print_stack(Stack *stack) {
    unsigned char i;
    for (i = 0; i < stack->top; i++) {
        printf("%d: %g\n", i, stack_get(stack, i));
    }
}

int process(Stack *stack, char *input) {
    unsigned char i;
    for (i = 0; input[i]; i++) input[i] = (char)tolower(input[i]);
    if (!strcmp(input, "q")) {
        exit(0);
    } else if (!strcmp(input, "+")) {
        if (stack_add(stack)) return 1;
    } else if (!strcmp(input, "-")) {
        if (stack_sub(stack)) return 1;
    } else if (!strcmp(input, "*")) {
        if (stack_mul(stack)) return 1;
    } else if (!strcmp(input, "/")) {
        if (stack_div(stack)) return 1;
    } else if (!strcmp(input, "%")) {
        if (stack_mod(stack)) return 1;
    } else if (!strcmp(input, "pow")) {
        if (stack_pow(stack)) return 1;
    } else if (!strcmp(input, "sqrt")) {
        if (stack_sqrt(stack)) return 1;
    } else if (!strcmp(input, "neg")) {
        if (stack_neg(stack)) return 1;
    } else if (!strcmp(input, "round")) {
        if (stack_round(stack)) return 1;
    } else if (!strcmp(input, "!")) {
        if (stack_fac(stack)) return 1;
    } else if (!strcmp(input, "sort")) {
        stack_sort(stack);
    } else if (!strcmp(input, "clear")) {
        stack_clear(stack);
    } else if (!strcmp(input, "rev")) {
        stack_rev(stack);
    } else if (!strcmp(input, "drop")) {
        if (stack->top) stack->top--;
    } else if (!strlen(input)) {
        stack_push(stack, stack->top ? stack_get(stack, stack->top - 1) : 0.);
    } else {
        char *end;
        double val = strtod(input, &end);
        if (input == end || *end != '\0') {
            printf("Unknown operation: %s.\n", input);
        } else {
            stack_push(stack, val);
        }
    }
    return 0;
}

void warranty() {
    printf(
        "THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n"
        "APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE "
        "COPYRIGHT\n"
        "HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" "
        "WITHOUT "
        "WARRANTY\n"
        "OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT "
        "LIMITED "
        "TO,\n");
    printf(
        "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A "
        "PARTICULAR\n"
        "PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF "
        "THE "
        "PROGRAM\n"
        "IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE "
        "COST "
        "OF\n"
        "ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n");
}
