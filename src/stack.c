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

#ifdef USE_FLOAT
#define floor_t(x) floorf(x)
#define pow_t(x, y) powf(x, y)
#define sqrt_t(x) sqrtf(x)
#else
#define floor_t(x) floor(x)
#define pow_t(x, y) pow(x, y)
#define sqrt_t(x) sqrt(x)
#endif

#include "stack.h"

#include <math.h>
#include <stdlib.h>

int stack_init(Stack *stack) {
    Float *ptr = (Float *)malloc(sizeof(Float) * MAX_SIZE);
    if (ptr != NULL) {
        stack->array = ptr;
        stack->len = 0;
        return 0;
    } else
        return 1;
}

int stack_push(Stack *stack, Float val) {
    if (stack->len == MAX_SIZE) return 1;
    stack->array[stack->len++] = val;
    return 0;
}

Float stack_pop(Stack *stack) {
    return stack->len ? stack->array[--stack->len] : 0.0;
}

Float stack_get(Stack *stack, unsigned char i) {
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
    Float a = stack_pop(stack), b = stack_pop(stack);
    return stack_push(stack, b / a);
}

int stack_pow(Stack *stack) {
    Float a = stack_pop(stack), b = stack_pop(stack);
    return stack_push(stack, pow_t(b, a));
}

int stack_sqrt(Stack *stack) {
    return stack_push(stack, sqrt_t(stack_pop(stack)));
}

int stack_neg(Stack *stack) {
    Float val = stack_pop(stack);
    /* -0 workaround */
    return stack_push(stack, -(val != 0.0f) * val);
}

static Float round_t(Float val) {
    Float floor_val = floor_t(val), decimal = val - floor_val;
    return floor_val + (val >= 0.0f ? decimal >= 0.5f : decimal > 0.5f);
}

int stack_round(Stack *stack) {
    Float digit = floor_t(stack_pop(stack)), val = stack_pop(stack);
    return stack_push(stack,
                      round_t(val * pow_t(10, digit)) / pow_t(10, digit));
}

/* TODO: use gamma function instead */
int stack_fac(Stack *stack) {
    int i;
    Float val = floor_t(stack_pop(stack)), sum = 1.;

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
        Float temp = stack->array[i];
        stack->array[i] = stack->array[j];
        stack->array[j] = temp;
    }
}

/* for order, IEEE 754 compliance is assumed and any value < NAN = NAN */
static int ord(const void *va, const void *vb) {
    Float a = *(const Float *)va, b = *(const Float *)vb;
    if (a != a) /* is a NAN? */
        return b != b ? 0 : 1;
    else if (b != b || a < b)
        return -1;
    else if (a > b)
        return 1;
    return 0;
}

void stack_sort(Stack *stack) {
    qsort(stack->array, stack->len, sizeof(Float), ord);
}
