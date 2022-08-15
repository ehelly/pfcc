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

#include <limits.h>

#if !defined(MAX_SIZE) || MAX_SIZE > UCHAR_MAX
#define MAX_SIZE UCHAR_MAX
#endif

#pragma GCC diagnostic ignored "-Wpadded"
typedef struct Stack {
    double *array;
    unsigned char top;
} Stack;

int stack_init(Stack *stack);

int stack_push(Stack *stack, double val);
double stack_pop(Stack *stack);

double stack_get(Stack *stack, unsigned char i);

void stack_clear(Stack *stack);

int stack_add(Stack *stack);
int stack_sub(Stack *stack);
int stack_mul(Stack *stack);
int stack_div(Stack *stack);
int stack_mod(Stack *stack);
int stack_pow(Stack *stack);
int stack_sqrt(Stack *stack);

int stack_neg(Stack *stack);

int stack_round(Stack *stack);

int stack_fac(Stack *stack);
void stack_rev(Stack *stack);

void stack_sort(Stack *stack);

#endif
