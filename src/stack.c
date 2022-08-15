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

#include <limits.h>
#include <math.h>
#include <stdlib.h>

int stack_init(Stack *stack) {
    double *ptr = (double *)malloc(sizeof(double) * MAX_SIZE);
    if (ptr != NULL) {
        stack->array = ptr;
        stack->top = 0;
        return 0;
    } else
        return 1;
}

int stack_push(Stack *stack, double val) {
    if (stack->top == MAX_SIZE) return 1;
    stack->array[stack->top++] = val;
    return 0;
}

double stack_pop(Stack *stack) {
    return stack->top ? stack->array[--stack->top] : 0.0;
}

double stack_get(Stack *stack, unsigned char i) {
    return i < stack->top ? stack->array[i] : 0.0;
}

void stack_clear(Stack *stack) { stack->top = 0; }

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

static int float_to_int(double val) {
    return val == val && (double)INT_MIN - 1.0f < val &&
                   val < (double)INT_MAX + 1.0f
               ? (int)val
               : 0;
}

int stack_mod(Stack *stack) {
    int a = float_to_int(stack_pop(stack)), b = float_to_int(stack_pop(stack));
    return stack_push(stack, (double)(b % a));
}

int stack_pow(Stack *stack) {
    double a = stack_pop(stack), b = stack_pop(stack);
    return stack_push(stack, pow(b, a));
}

int stack_sqrt(Stack *stack) {
    return stack_push(stack, sqrt(stack_pop(stack)));
}

int stack_neg(Stack *stack) {
    double val = stack_pop(stack);
    /* -0 workaround */
    return stack_push(stack, -(val != 0.0f) * val);
}

static double round_t(double val) {
    double floor_val = floor(val), decimal = val - floor_val;
    return floor_val + (val >= 0.0f ? decimal >= 0.5f : decimal > 0.5f);
}

int stack_round(Stack *stack) {
    double digit = floor(stack_pop(stack)), val = stack_pop(stack);
    return stack_push(stack, round_t(val * pow(10, digit)) / pow(10, digit));
}

/* TODO: use gamma function instead */
int stack_fac(Stack *stack) {
    int i;
    double val = floor(stack_pop(stack)), sum = 1.;

    if (val < 0) {
        stack_push(stack, 0. / 0.);
        return 0;
    }

    /* slow, naive implementation */
    for (i = 1; i < val; sum *= ++i) continue;
    return stack_push(stack, sum);
}

void stack_rev(Stack *stack) {
    unsigned char i = 0, j = stack->top - 1;
    for (; i < stack->top / 2; i++, j--) {
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
    qsort(stack->array, stack->top, sizeof(double), ord);
}
