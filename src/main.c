#include <stdio.h>
#include <unistd.h>
#include "mat.h"
#include "list.h"
#include "sph.h"

void
show(mat m)
{
  printf("(%d,%d)->[\n", m->n, m->m);
	for(int i = 0; i < m->m; i++) {
		for(int j = 0; j < m->n; j++) {
			printf(" %8.3f", m->v[i][j]);
		}
		printf("\n");
	}
	printf("]\n");
}

void
random_list(int m, int n, double v[][n])
{
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < n; j++) {
			v[i][j] = (double)rand() / (double)RAND_MAX;
		}
	}
}

int
main(void)
{
  //int m = 3;
  //int n = 2;
  //printf("alloc(m,n):     "); show(alloc(m, n));
  //printf("eye(m):         "); show(eye(m));
  //mat x = randm(m, m);
  //printf("x=randm(m,m):   "); show(x);
  //printf("transpose(x):   "); show(transpose(x));
	//printf("inverse(x):     "); show(inverse(x));
  //matlist xl = list_alloc();
	//xl->append(xl, x);
	//printf("list_get(0):    "); show(xl->x[0]);
	//xl->append(xl, randm(m,m));
	//printf("list_get(1):    "); show(xl->x[1]);	
	//list_del(xl);
	//del(x);
	puts("define");      double pos[400][3] = {0};
	puts("random_list"); random_list(400, 3, pos);
	puts("sph_alloc");
	sph s = sph_alloc(
		0.04,
		0.1,
		0.1,
		1.0,
		1.0,
		2/400,
		pos,
		400
	);
	puts("sph->init()"); s->init(s);
	puts("sph->step()"); s->step(s);
	return 0;
}