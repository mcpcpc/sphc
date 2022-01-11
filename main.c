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

void
test_matrix(void)
{
  int m = 3;
  int n = 2;
  printf("create(m,n):       "); show(create(m, n));
  printf("eye(m):            "); show(eye(m));
  mat x = randm(m, m);
  printf("x=randm(m,m):      "); show(x);
	printf("submat(x,0,1,0,1): "); show(submat(x,0,1,0,1));
  printf("transpose(x):      "); show(transpose(x));
	printf("add(x,x):          "); show(add(x,x));
	printf("sub(x,x):          "); show(sub(x,x));
	printf("power(x,2):        "); show(power(x,2));
	printf("exponent(x,2):     "); show(exponent(x,2));
	printf("inverse(x):        "); show(inverse(x));
	printf("multiply(x,x):     "); show(multiply(x,x));
	printf("divide(x,x):       "); show(divide(x,x));
	printf("distance(x,x):     "); show(distance(submat(x,0,2,0,0),transpose(submat(x,0,2,0,0))));
	matlist xl = list_create();
	xl->append(xl, x);
	printf("xl->[0]:           "); show(xl->x[0]);
	xl->append(xl, randm(m,m));
	printf("xl->[1]:           "); show(xl->x[1]);
	list_destroy(xl);
	destroy(x);
}

void
test_sph(void)
{
	double pos[10][3] = {0};
	random_list(10, 3, pos);
	puts("sph_create "); sph s = sph_create(0.04, 0.1, 0.1, 1.0, 1.0, 2/10, pos, 10);
	puts("sph->step()"); s->step(s);
	puts("sph_destroy"); sph_destroy(s);
}

void
test_sim(void)
{
	double pos[10][3] = {0};
	random_list(10, 3, pos);
	sph s = sph_create(0.04, 0.1, 0.1, 1.0, 1.0, 2/10, pos, 10);
}

int
main(void)
{
	//test_matrix();
	//test_sph();
	test_sim();
	return 0;
}