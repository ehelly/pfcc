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
    TYPE *ptr = (TYPE *)malloc(sizeof(TYPE) * MAX_SIZE);
    if (ptr != NULL) {
        stack->array = ptr;
        stack->len = 0;
        return 0;
    } else
        return 1;
}

int stack_push(Stack *stack, TYPE val) {
    if (stack->len == MAX_SIZE) return 1;
    stack->array[stack->len++] = val;
    return 0;
}

TYPE stack_pop(Stack *stack) {
    return stack->len ? stack->array[--stack->len] : 0.0;
}

TYPE stack_get(Stack *stack, unsigned char i) {
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
    TYPE a = stack_pop(stack), b = stack_pop(stack);
    return stack_push(stack, b / a);
}

int stack_pow(Stack *stack) {
    TYPE a = stack_pop(stack), b = stack_pop(stack);
#ifdef USE_FLOAT
    return stack_push(stack, powf(b, a));
#else
    return stack_push(stack, pow(b, a));
#endif
}

int stack_sqrt(Stack *stack) {
#ifdef USE_FLOAT
    return stack_push(stack, sqrtf(stack_pop(stack)));
#else
    return stack_push(stack, sqrt(stack_pop(stack)));
#endif
}

int stack_neg(Stack *stack) {
    TYPE val = stack_pop(stack);
    /* -0 workaround */
    return stack_push(stack, -(val != 0.0f) * val);
}

static TYPE floor_t(TYPE val) {
#ifdef USE_FLOAT
    return floorf(val);
#else
    return floor(val);
#endif
}

static TYPE round_t(TYPE val) {
    TYPE floor_val = floor_t(val), decimal = val - floor_val;
    return floor_val + (val >= 0.0f ? decimal >= 0.5f : decimal > 0.5f);
}

int stack_round(Stack *stack) {
    TYPE digit = floor_t(stack_pop(stack)), val = stack_pop(stack);
#ifdef USE_FLOAT
    return stack_push(stack, round_t(val * powf(10, digit)) / powf(10, digit));
#else
    return stack_push(stack, round_t(val * pow(10, digit)) / pow(10, digit));
#endif
}

/* TODO: use gamma function instead */
int stack_fac(Stack *stack) {
    int i;
    TYPE val = floor_t(stack_pop(stack)), sum = 1.;

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
        TYPE temp = stack->array[i];
        stack->array[i] = stack->array[j];
        stack->array[j] = temp;
    }
}

/* for order, IEEE 754 compliance is assumed and any value < NAN = NAN */
static int ord(const void *va, const void *vb) {
    TYPE a = *(const TYPE *)va, b = *(const TYPE *)vb;
    if (a != a) /* is a NAN? */
        return b != b ? 0 : 1;
    else if (b != b || a < b)
        return -1;
    else if (a > b)
        return 1;
    return 0;
}

void stack_sort(Stack *stack) {
    qsort(stack->array, stack->len, sizeof(TYPE), ord);
}
