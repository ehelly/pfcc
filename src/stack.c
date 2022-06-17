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

int stack_push(Stack *stack, double val) {
    if (stack->len == MAX_SIZE) return -1;
    stack->array[stack->len++] = val;
    return 0;
}

double stack_pop(Stack *stack) {
    return (!stack->len) ? 0.0 : stack->array[--stack->len];
}

double stack_get(Stack *stack, unsigned char i) {
    return (i >= stack->len) ? 0.0 : stack->array[i];
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
        stack_push(stack, (double)NAN);
        return 0;
    }

    /* slow, naive implementation */
    for (i = 1; i < val; sum *= ++i) continue;
    return stack_push(stack, sum);
}

static unsigned char partition(double array[], unsigned char low,
                               unsigned char high) {
    double pivot = array[(low + high) / 2];

    while (1) {
        double tmp;

        for (; array[low] < pivot; low++) continue;

        for (; array[high] > pivot; high--) continue;

        if (low >= high) return high;

        /* swap */
        tmp = array[low];
        array[low] = array[high];
        array[high] = tmp;
    }
}

static void quicksort(double array[], unsigned char low, unsigned char high) {
    if (low < high) {
        unsigned char p = partition(array, low, high);
        quicksort(array, low, p);
        quicksort(array, p + 1, high);
    }
}

void stack_sort(Stack *stack) {
    if (stack->len == 0) return;
    quicksort(stack->array, 0, stack->len - 1);
}
