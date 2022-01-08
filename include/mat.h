
#ifndef __MAT_H
#define __MAT_H

#include <stdlib.h>
#include <math.h>

typedef struct {
	int m, n;
	double ** v;
} mat_t, *mat;

mat alloc(int m, int n) {
	mat x = malloc(sizeof(mat_t));
	x->v = malloc(sizeof(double*) * m);
	x->v[0] = calloc(sizeof(double), m * n);
	for (int i = 0; i < m; i++) {
		x->v[i] = x->v[0] + n * i;
  }
	x->m = m;
	x->n = n;
	return x;
}

void del(mat x) {
	free(x->v[0]);
	free(x->v);
	free(x);
}

mat eye(int m) {
	mat x = alloc(m, m);
	for (int i = 0; i < x->m; i++) {
		x->v[i][i] = 1.0;
	}
	return x;
}

mat randm(int m, int n) {
	mat x = alloc(m, n);
	for (int i = 0; i < x->m; i++) {
		for (int j = 0; j < x->n; j++) {
			x->v[i][j] = (double)rand() / (double)RAND_MAX;
    }
	}
	return x;
}

void transpose(mat x) {
	double t = 0.0;
	for (int i = 0; i < x->m; i++) {
		for (int j = 0; j < i; j++) {
			t = x->v[i][j];
			x->v[i][j] = x->v[j][i];
			x->v[j][i] = t;
		}
	}
}

mat copy(int n, double a[][n], int m) {
	mat x = alloc(m, n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			x->v[i][j] = a[i][j];
		}
	}
	return x;
}

mat duplicate(mat x) {
	mat y = alloc(x->m, x->n);
	for (int i = 0; i < x->m; i++) {
		for (int j = 0; j < x->n; j++) {
			y->v[i][j] = x->v[i][j];
		}
	}
	return y;
}

mat scale(mat x, double n) {
	mat y = alloc(x->m, x->n);
	for (int i = 0; i < x->m; i++) {
		for (int j = 0; j < x->n; j++) {
			y->v[i][j] = x->v[i][j] * n;
  	}
	}
	return y;
}

mat add(mat x1, mat x2) {
	mat y = alloc(x1->m, x1->n);
	for (int i = 0; i < x1->m; i++) {
		for (int j = 0; j < x1->n; j++) {
			y->v[i][j] = x1->v[i][j] + x2->v[i][j];
  	}
	}
	return y;
}

mat sub(mat x1, mat x2) {
	mat y = alloc(x1->m, x1->n);
	for (int i = 0; i < x1->m; i++) {
		for (int j = 0; j < x1->n; j++) {
			y->v[i][j] = x1->v[i][j] + x2->v[i][j];
  	}
	}
	return y;
}

mat power(mat x, double n) {
	mat y = alloc(x->m, x->n);
	for (int i = 0; i < x->m; i++) {
		for (int j = 0; j < x->n; j++) {
			y->v[i][j] = pow(x->v[i][j], n);
  	}
	}
	return y;
}

mat multiply(mat x1, mat x2) {
	if (x1->n != x2->m) return 0;
	mat y = alloc(x1->m, x2->n);
	for (int i = 0; i < x1->m; i++) {
		for (int j = 0; j < x2->n; j++) {
			for (int k = 0; k < x1->n; k++) {
				y->v[i][j] += x1->v[i][k] * x2->v[k][j];
			}
  	}
	}
	return y;
}

mat submat(mat x, int m1, int m2, int n1, int n2) {
    mat y = alloc(m2 - m1 + 1, n2 - n1 + 1);
    for (int m = 0; m < y->m; m++) {
        for (int n = 0; n < y->n; n++) {
            y->v[m][n] = x->v[m + 1][n + n1];
        }
    }
    return y;
}

double det(mat x) {
  if (x->m != x->n) exit(2);
  double d = 0;
  double r = 0;
  int i, j, k;
  mat a = duplicate(x);
  for(i = 0;i < x->n; i++) {
    if(a->[i][i] == 0.0) exit(0);
    for(j = i + 1; j < x->n; j++) {
      r = a->v[j][i] / a->a[i][i];
      for(k = 0; k < x->n; k++) {
        a->v[j][k] = a->[j][k] - r*a->[i][k];
      }
    }
  }
  for (i = 0; i < x->n; i++) {
    d = d * a->v[i][i];
  }
  del(a);
  return d;
}

/*

mat adjoin(mat x) {

}

mat inverse(mat x) {
	mat y = scale(adjoin(x),1/det(x));
  return y;
}

mat divide(mat x1, mat x2) {
	mat y = multiply(x1, inverse(x2));
	return y;
}
*/
#endif