#include "list.h"

matlist list_alloc(void) {
	matlist xl = malloc(sizeof(matlist_t));
	xl->n = 0;
	xl->list_add = list_add;
	return xl;
}

mat list_get(matlist xl, int i) {
	mat x = duplicate(xl->x[i]);
	return x;
}

void list_set(matlist xl, int i, mat x) {
	xl->x[i] = duplicate(x);
}

void list_add(matlist xl, mat x) {
	xl->x = realloc(xl->x, sizeof(xl->x) + sizeof(x));
	xl->n += 1;
	xl->x[xl->n - 1] = duplicate(x);
}
/*
void list_drop(matlist xl, int i) {
	
}
*/
void list_del(matlist xl) {
	for (int i = 0; i < xl->n; i++) del(xl->x[i]);
	free(xl->x);
	free(xl);
}