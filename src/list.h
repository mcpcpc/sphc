#ifndef __LIST_H
#define __LIST_H

#include "mat.h"

typedef struct MATRIX_LIST {
	int n;
	mat *x;
	void (*append)(struct MATRIX_LIST *self, mat x);
} matlist_t, *matlist;

matlist list_create(void);
void list_destroy(matlist xl);

#endif
