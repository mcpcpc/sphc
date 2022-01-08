
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
	for(int i = 0; i < m; i++) {
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
	for(int i = 0; i < x->m; i++) {
		x->v[i][i] = 1.0;
	}
	return x;
}

mat randm(int m, int n) {
	mat x = alloc(m, n);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			x->v[i][j] = (double)rand() / (double)RAND_MAX;
    }
	}
	return x;
}

mat transpose(mat x) {
	mat y = alloc(x->n, x->m);
  for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			y->v[j][i] = x->v[i][j];
		}
	}
  return y;
}

mat copy(int n, double a[][n], int m) {
	mat x = alloc(m, n);
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < n; j++) {
			x->v[i][j] = a[i][j];
		}
	}
	return x;
}

mat duplicate(mat x) {
	mat y = alloc(x->m, x->n);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			y->v[i][j] = x->v[i][j];
		}
	}
	return y;
}

mat scale(mat x, double n) {
	mat y = alloc(x->m, x->n);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			y->v[i][j] = x->v[i][j] * n;
  	}
	}
	return y;
}

mat add(mat x1, mat x2) {
	mat y = alloc(x1->m, x1->n);
	for(int i = 0; i < x1->m; i++) {
		for(int j = 0; j < x1->n; j++) {
			y->v[i][j] = x1->v[i][j] + x2->v[i][j];
  	}
	}
	return y;
}

mat sub(mat x1, mat x2) {
	mat y = alloc(x1->m, x1->n);
	for(int i = 0; i < x1->m; i++) {
		for(int j = 0; j < x1->n; j++) {
			y->v[i][j] = x1->v[i][j] + x2->v[i][j];
  	}
	}
	return y;
}

mat power(mat x, double n) {
	mat y = alloc(x->m, x->n);
	for(int i = 0; i < x->m; i++) {
		for(int j = 0; j < x->n; j++) {
			y->v[i][j] = pow(x->v[i][j], n);
  	}
	}
	return y;
}

mat multiply(mat x1, mat x2) {
	if (x1->n != x2->m) return 0;
	mat y = alloc(x1->m, x2->n);
	for(int i = 0; i < x1->m; i++) {
		for(int j = 0; j < x2->n; j++) {
			for(int k = 0; k < x1->n; k++) {
				y->v[i][j] += x1->v[i][k] * x2->v[k][j];
			}
  	}
	}
	return y;
}

mat submat(mat x, int m1, int m2, int n1, int n2) {
    mat y = alloc(m2 - m1 + 1, n2 - n1 + 1);
    for(int m = 0; m < y->m; m++) {
      for(int n = 0; n < y->n; n++) {
        y->v[m][n] = x->v[m + 1][n + n1];
      }
    }
    return y;
}

void delm(mat x1, mat x2, int m){	
	for(int i = 0; i < x2->m; i++) {
		for(int j = 0; j < x2->n; j++) {
			if(i < m) x2->v[i][j] = x1->v[i][j];
			else x2->v[i][j] = x1->v[i+1][j];
    }
  }	
}

void deln(mat x1, mat x2, int n) {	
  //int k = 0;
  //for(int i = 0; i < x1->m; i++){
  //  for(int j = 0; j < x1->n; j++){
  //    if(j != n) {
  //      x2->v[k % x2->n][k / x2->n] = x1->v[i][j];				
  //      k++;
  for(int i = 0; i < x2->m; i++) {
		for(int j = 0; j < x2->n; j++) {
			if(j < n) x2->v[i][j] = x1->v[i][j];
			else x2->v[i][j] = x1->v[i][j+1];	
 //     }
    }
  }	
}

double determinant(mat x) {
    double d = 0;
    if ((x->m == 1) && (x->n == 1)) {
        d = x->v[0][0];
    } else {
        double si = 1;
        mat y1 = submat(x, 1, x->m - 1, 0, x->n -1);
        mat y2 = alloc(y1->m, y1->n - 1);
        for (int i = 0; i < x->n; i++) {
            deln(y1, y2, i);
            d += si * determinant(y2) * x->v[i % x->n][i / x->n];
            si *= -1;
        }
        del(y1);
        del(y2);
    }
    return d;
}

mat adjoint(mat x) {
  mat B = alloc(x->m, x->n);
  mat A1= alloc(x->m - 1, x->n);
  mat A2= alloc(x->m - 1, x->n - 1);
  for (int i = 1; i < x->m; i++){
    delm(x, A1, i);
    for (int j = 0; j < x->n; j++){            
      deln(A1, A2, j);
      B->v[i][j] = determinant(A2) * (double)pow(-1, (double)(i+j));         
    }
  }
  del(A2);
  del(A1);
  mat y = transpose(B);
  del(B);
  return y;
}

mat inverse(mat x) {
	mat y = scale(adjoint(x),1/determinant(x));
  return y;
}

/*


double det(mat x) {
  if(x->m != x->n) exit(0);
  double d = 0;
  double sign = 1;
  for(int i = 0; i < x->m; i++) {
    mat y = cofactor();
    d += sign * x[0][fi] * det(y);
  }
  return d;
}


mat gaussian_elimination(mat x) {
  double r = 0;
  mat y = duplicate(x);
  for(int i = 0; i < x->n; i++) {
    if(y->v[i][i] == 0.0) exit(0);
    for(int j = i + 1; j < x->n; j++) {
      r = y->v[j][i] / y->v[i][i];
      for(int k = 0; k < x->n; k++) {
        y->v[j][k] = y->v[j][k] - r*y->v[i][k];
      }
    }
  }
  return y;
}

double det(mat x) {
  double d = 1.0;
  if (x->m != x->n) exit(0);
  for (int i = 0; i < x->m; i++) {
    d *= x->v[i][i];
  }
  return d;
}

mat divide(mat x1, mat x2) {
	mat y = multiply(x1, inverse(x2));
	return y;
}

*/
#endif