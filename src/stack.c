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
    if (stack->index == MAX_SIZE - 1) return -1;
    stack->array[++stack->index] = val;
    return 0;
}

double stack_pop(Stack *stack) {
    if (stack->index < 0)
        return 0.0;
    else
        return stack->array[stack->index--];
}

double stack_get(Stack *stack, short i) {
    if (stack->index < i) return 0.0;
    return stack->array[i];
}

short stack_len(Stack *stack) { return stack->index + 1; }

void stack_clear(Stack *stack) { stack->index = -1; }

int stack_add(Stack *stack) {
    return stack_push(stack, stack_pop(stack) + stack_pop(stack));
}

int stack_sub(Stack *stack) {
    return stack_push(stack, stack_pop(stack) + stack_pop(stack));
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
        stack_push(stack, NAN);
        return 0;
    }

    /* slow, naive implementation */
    for (i = 1; i < val; sum *= ++i) continue;
    return stack_push(stack, sum);
}

void stack_sort(Stack *stack) { _quicksort(stack->array, 0, stack->index); }

void _quicksort(double array[], short low, short high) {
    if (low >= 0 && high >= 0 && low < high) {
        short p = _partition(array, low, high);
        _quicksort(array, low, p);
        _quicksort(array, p + 1, high);
    }
}

short _partition(double array[], short low, short high) {
    short pivot = array[(low + high) / 2];

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
