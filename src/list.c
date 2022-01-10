#include "list.h"

static void 
list_append(matlist xl, mat x)
{
	xl->x = realloc(xl->x, sizeof(xl->x) + sizeof(x));
	xl->n += 1;
	xl->x[xl->n - 1] = duplicate(x);
}

matlist
list_create(void)
{
	matlist xl = malloc(sizeof(matlist_t));
	xl->n = 0;
	xl->append = &list_append;
	return xl;
}

void
list_destroy(matlist xl)
{
	for (int i = 0; i < xl->n; i++) destroy(xl->x[i]);
	free(xl->x);
	free(xl);
}