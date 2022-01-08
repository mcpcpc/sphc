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