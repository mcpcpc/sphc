#ifndef __LIST_H
#define __LIST_H

#include "mat.h"

typedef struct MATRIX_LIST {
	int n;
	mat *x;
	void * (*list_add)(struct MATRIX_LIST *self, mat x);
} matlist_t, *matlist;

matlist list_alloc(void);
void list_add(matlist xl, mat x);
mat list_get(matlist xl, int i);
void list_set(matlist xl, int i, mat x);
//void list_drop(matlist xl, int i);
void list_del(matlist xl);

#endif