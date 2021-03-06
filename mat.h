#ifndef __MAT_H
#define __MAT_H

#include <stdlib.h>
#include <math.h>

typedef struct MATRIX {
	int m, n;
	double ** v;
} mat_t, *mat;

mat create(int m, int n);
void destroy(mat x);
mat eye(int m);
mat randm(int m, int n);
mat transpose(mat x);
mat copy(int n, double a[][n], int m);
mat duplicate(mat x);
mat scale(mat x, double n);
mat add(mat x1, mat x2);
mat sub(mat x1, mat x2);
mat power(mat x, double n);
mat exponent(mat x, double n);
mat multiply(mat x1, mat x2);
mat submat(mat x, int m1, int m2, int n1, int n2);
void delm(mat x1, mat x2, int m);
void deln(mat x1, mat x2, int n);
double determinant(mat x);
mat adjoint(mat x);
mat inverse(mat x);
mat divide(mat x1, mat x2);
mat sum(mat x, int axis);
mat vstack(mat x1, mat x2);
mat hstack(mat x1, mat x2);
mat distance(mat x1, mat x2);

#endif
