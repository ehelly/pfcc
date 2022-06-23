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

#ifndef STACK_H
#define STACK_H

#ifndef MAX_SIZE
#define MAX_SIZE 255
#endif

#ifdef USE_FLOAT
#define TYPE float
#else
#define TYPE double
#endif

#pragma GCC diagnostic ignored "-Wpadded"
typedef struct Stack {
    TYPE *array;
    unsigned char len;
} Stack;

int stack_init(Stack *stack);

int stack_push(Stack *stack, TYPE val);
TYPE stack_pop(Stack *stack);

TYPE stack_get(Stack *stack, unsigned char i);

void stack_clear(Stack *stack);

int stack_add(Stack *stack);
int stack_sub(Stack *stack);
int stack_mul(Stack *stack);
int stack_div(Stack *stack);
int stack_pow(Stack *stack);

int stack_fac(Stack *stack);
void stack_rev(Stack *stack);

void stack_sort(Stack *stack);
/* void _quicksort(double array[], short lo, short hi);
short _partition(double array[], short lo, short hi); */

#endif
