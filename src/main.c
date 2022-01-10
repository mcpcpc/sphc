#include <stdio.h>
#include <unistd.h>
#include "mat.h"
#include "list.h"
//#include "sph.h"

// https://www.codeproject.com/Articles/5283245/Matrix-Library-in-C
// https://rosettacode.org/wiki/QR_decomposition#C
// https://rosettacode.org/wiki/Determinant_and_permanent#C
// https://stackoverflow.com/questions/27003062/fastest-algorithm-for-computing-the-determinant-of-a-matrix
// https://www.codewithc.com/c-program-for-lu-factorization/
// https://www.geeksforgeeks.org/determinant-of-a-matrix/
// https://www.codesansar.com/c-programming-examples/matrix-determinant.htm

void show(mat m) {
  printf("(%d,%d)->[\n", m->n, m->m);
	for(int i = 0; i < m->m; i++) {
		for (int j = 0; j < m->n; j++) {
			printf(" %8.3f", m->v[i][j]);
		}
		printf("\n");
	}
	printf("]\n");
}

int main(void) {
  int m = 3;
  int n = 2;
  printf("alloc(m,n):     "); show(alloc(m, n));
  printf("eye(m):         "); show(eye(m));
  mat x = randm(m, m);
  printf("x=randm(m,m):   "); show(x);
  printf("transpose(x):   "); show(transpose(x));
	printf("inverse(x):     "); show(inverse(x));
  matlist xl = list_alloc();
	xl->append(xl, x);
	printf("list_get(0):    "); show(xl->x[0]);
	xl->append(xl, randm(m,m));
	printf("list_get(1):    "); show(xl->x[1]);	
	list_del(xl);
	del(x);
	return 0;
}