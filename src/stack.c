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

#include "stack.h"

#include <math.h>
#include <stdlib.h>

int stack_init(Stack *stack) {
    double *ptr = (double *)malloc(sizeof(double) * MAX_SIZE);
    if (ptr != NULL) {
        stack->array = ptr;
        stack->len = 0;
        return 1;
    } else
        return 0;
}

int stack_push(Stack *stack, double val) {
    if (stack->len == MAX_SIZE) return 1;
    stack->array[stack->len++] = val;
    return 0;
}

double stack_pop(Stack *stack) {
    return stack->len ? stack->array[--stack->len] : 0.0;
}

double stack_get(Stack *stack, unsigned char i) {
    return i < stack->len ? stack->array[i] : 0.0;
}

void stack_clear(Stack *stack) { stack->len = 0; }

int stack_add(Stack *stack) {
    return stack_push(stack, stack_pop(stack) + stack_pop(stack));
}

int stack_sub(Stack *stack) {
    return stack_push(stack, -stack_pop(stack) + stack_pop(stack));
}

int stack_mul(Stack *stack) {
    return stack_push(stack, stack_pop(stack) * stack_pop(stack));
}

int stack_div(Stack *stack) {
    double a = stack_pop(stack), b = stack_pop(stack);
    return stack_push(stack, b / a);
}

int stack_pow(Stack *stack) {
    double a = stack_pop(stack), b = stack_pop(stack);
    return stack_push(stack, pow(b, a));
}

/* TODO: use gamma function instead */
int stack_fac(Stack *stack) {
    int i;
    double val = floor(stack_pop(stack)), sum = 1;

    if (val < 0) {
        stack_push(stack, 0. / 0.);
        return 0;
    }

    /* slow, naive implementation */
    for (i = 1; i < val; sum *= ++i) continue;
    return stack_push(stack, sum);
}

void stack_rev(Stack *stack) {
    unsigned char i = 0, j = stack->len - 1;
    for (; i < stack->len / 2; i++, j--) {
        double temp = stack->array[i];
        stack->array[i] = stack->array[j];
        stack->array[j] = temp;
    }
}

/* for order, IEEE 754 compliance is assumed and any value < NAN = NAN */
static int ord(const void *va, const void *vb) {
    double a = *(const double *)va, b = *(const double *)vb;
    if (a != a) /* is a NAN? */
        return b != b ? 0 : 1;
    else if (b != b || a < b)
        return -1;
    else if (a > b)
        return 1;
    return 0;
}

void stack_sort(Stack *stack) {
    qsort(stack->array, stack->len, sizeof(double), ord);
}
