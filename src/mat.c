#include "mat.h"

mat
create(int m, int n)
{
	mat x = malloc(sizeof(mat_t));
	x->v = malloc(sizeof(double*) * m);
	x->v[0] = calloc(sizeof(double), m * n);
	for(int i = 0; i < m; i++) {
		x->v[i] = x->v[0] + n * i;
	}
	x->m = m;
	x->n = n;
	return x;
}

void
destroy(mat x)
{
	free(x->v[0]);
	free(x->v);
	free(x);
}

mat
eye(int m)
{
	mat x = create(m, m);
	for(int i = 0; i < x->m; i++) {
		x->v[i][i] = 1.0;
	}
	return x;
}

mat
randm(int m, int n)
{
	mat x = create(m, n);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			x->v[i][j] = (double)rand() / (double)RAND_MAX;
		}
	}
	return x;
}

mat
transpose(mat x)
{
	mat y = create(x->n, x->m);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			y->v[j][i] = x->v[i][j];
		}
	}
	return y;
}

mat
copy(int n, double a[][n], int m)
{
	mat x = create(m, n);
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < n; j++) {
			x->v[i][j] = a[i][j];
		}
	}
	return x;
}

mat
duplicate(mat x)
{
	mat y = create(x->m, x->n);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			y->v[i][j] = x->v[i][j];
		}
	}
	return y;
}

mat
scale(mat x, double n)
{
	mat y = create(x->m, x->n);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			y->v[i][j] = x->v[i][j] * n;
		}
	}
	return y;
}

mat
add(mat x1, mat x2)
{
	mat y = create(x1->m, x1->n);
	for(int i = 0; i < x1->m; i++) {
		for(int j = 0; j < x1->n; j++) {
			y->v[i][j] = x1->v[i][j] + x2->v[i][j];
		}
	}
	return y;
}

mat
sub(mat x1, mat x2)
{
	mat y = create(x1->m, x1->n);
	for(int i = 0; i < x1->m; i++) {
		for(int j = 0; j < x1->n; j++) {
			y->v[i][j] = x1->v[i][j] - x2->v[i][j];
		}
	}
	return y;
}

mat
power(mat x, double n)
{
	mat y = create(x->m, x->n);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			y->v[i][j] = pow(x->v[i][j], n);
		}
	}
	return y;
}

mat
exponent(mat x, double n)
{
	mat y = create(x->m, x->n);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			y->v[i][j] = pow(n, x->v[i][j]);
		}
	}
	return y;
}

mat
multiply(mat x1, mat x2)
{
	if (x1->n != x2->m) return 0;
	mat y = create(x1->m, x2->n);
	for(int i = 0; i < x1->m; i++) {
		for(int j = 0; j < x2->n; j++) {
			for(int k = 0; k < x1->n; k++) {
				y->v[i][j] += x1->v[i][k] * x2->v[k][j];
			}
		}
	}
	return y;
}

mat
submat(mat x, int m1, int m2, int n1, int n2)
{
	mat y = create(m2 - m1 + 1, n2 - n1 + 1);
	for(int m = 0; m < y->m; m++) {
		for(int n = 0; n < y->n; n++) {
			y->v[m][n] = x->v[m + m1][n + n1];
		}
	}
	return y;
}

void
delm(mat x1, mat x2, int m)
{	
	for(int i = 0; i < x2->m; i++) {
		for(int j = 0; j < x2->n; j++) {
			if (i < m) x2->v[i][j] = x1->v[i][j];
			else x2->v[i][j] = x1->v[i+1][j];
		}
	}	
}

void
deln(mat x1, mat x2, int n)
{
	for(int i = 0; i < x2->m; i++) {
		for(int j = 0; j < x2->n; j++) {
			if(j < n) x2->v[i][j] = x1->v[i][j];
			else x2->v[i][j] = x1->v[i][j+1];
		}
	}	
}

double
determinant(mat x)
{
	double d = 0;
	if ((x->m == 1) && (x->n == 1)) {
		d = x->v[0][0];
	} else {
		double si = 1;
		mat y1 = submat(x, 1, x->m - 1, 0, x->n -1);
		mat y2 = create(y1->m, y1->n - 1);
		for(int i = 0; i < x->n; i++) {
			deln(y1, y2, i);
			d += si * determinant(y2) * x->v[i % x->n][i / x->n];
			si *= -1;
		}
		destroy(y1);
		destroy(y2);
	}
	return d;
}

mat
adjoint(mat x)
{
	mat B = create(x->m, x->n);
	mat A1= create(x->m - 1, x->n);
	mat A2= create(x->m - 1, x->n - 1);
	for(int i = 0; i < x->m; i++){
		delm(x, A1, i);
		for(int j = 0; j < x->n; j++){            
			deln(A1, A2, j);
			B->v[i][j] = determinant(A2) * (double)pow(-1, (double)(i+j));         
		}
	}
	destroy(A2);
	destroy(A1);
	mat y = transpose(B);
	destroy(B);
	return y;
}

mat
inverse(mat x)
{
	mat y = scale(adjoint(x),1/determinant(x));
	return y;
}

mat
divide(mat x1, mat x2)
{
	mat y = multiply(x1, inverse(x2));
	return y;
}

mat
sum(mat x, int axis)
{
	if (axis == 0) {
		mat y = create(1, x->n);
		for(int i = 0; i < x->m; i++) {
			for(int j = 0; j < x->n; j ++) {
				y->v[1][j] += x->v[i][j];
			}
		}
		return y;
	} else if (axis == 1) {
		mat y = create(x->m, 1);
		for(int i = 0; i < x->m; i++) {
			for(int j = 0; j < x->n; j ++) {
				y->v[i][1] += x->v[i][j];
			}
		}
		return y;
	} else exit(0);
}

mat
vstack(mat x1, mat x2)
{
	if (x1->n != x2->n) exit(0);
	mat y = create(x1->m + x2->m, x1->n);
	for(int i = 0; i < y->m; i++) {
		for(int j = 0; j < y->n; j++) {
			if (i < x1->m) y->v[i][j] = x1->v[i][j];
			else y->v[i][j] = x2->v[i - x1->m][j];
		}
	}
	return y;
}

mat
hstack(mat x1, mat x2)
{
	if (x1->m != x2->m) exit(0);
	mat y = create(x1->m, x1->n + x2->n);
	for(int i = 0; i < y->m; i++) {
		for(int j = 0; j < y->n; j++) {
			if (j < x1->n) y->v[i][j] = x1->v[i][j];
			else y->v[i][j] = x2->v[i][j - x1->n];
		}
	}
	return y;
}
