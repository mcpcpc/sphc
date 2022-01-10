#include "sph.h"
#include <stdio.h>

static mat
w(mat dx, mat dy, mat dz, double h)
{
	mat r = power(add(add(power(dx, 2), power(dy, 2)), 
		power(dz, 2)), (1 / 2));
	double c = pow(1.0 / h * pow(M_PI, 1/2), 3);
	mat w = scale(exponent(scale(power(r, 2),
		1 / (h * h)), M_E), c);
	destroy(r);
	return w;
}

static matlist
gradw(mat x, mat y, mat z, double h)
{
	mat r = power(add(add(power(x ,2), power(y, 2)),
		power(z, 2)), (1 / 2));
	double c = -2 * pow(h, 5) / pow(M_PI, (3 / 2));
	mat n = scale(exponent(scale(scale(power(r, 2), -1),
		1 / h * h ), M_E), c);
	matlist wxyz = list_create();
	wxyz->append(wxyz, multiply(n,x));
	wxyz->append(wxyz, multiply(n,y));
	wxyz->append(wxyz, multiply(n,z));
	destroy(r);
	destroy(n);
	return wxyz;
}

static matlist
pairwiseseparation(mat ri, mat rj)
{
	mat rix = submat(ri, 0, ri->m - 1, 0, 0);
	mat riy = submat(ri, 0, ri->m - 1, 1, 1);
	mat riz = submat(ri, 0, ri->m - 1, 2, 2);
	mat rjx = submat(rj, 0, rj->m - 1, 0, 0);
	mat rjy = submat(rj, 0, rj->m - 1, 1, 1);
	mat rjz = submat(rj, 0, rj->m - 1, 2, 2);
	matlist dxyz = list_create();
	dxyz->append(dxyz, sub(rix,rjx));
	dxyz->append(dxyz, sub(riy,rjy));
	dxyz->append(dxyz, sub(riz,rjz));
	destroy(rix);
	destroy(riy);
	destroy(riz);
	destroy(rjx);
	destroy(rjy);
	destroy(rjz);
	return dxyz;

}
static mat
density(mat r, mat pos, double m, double h)
{
	matlist dxyz = pairwiseseparation(r, pos);
	mat dw = w(dxyz->x[0], dxyz->x[0], dxyz->x[0], h);
	//puts("make t"); mat t = scale(dw, m);
	//printf("t: %d, %d\n", t->m, t->n);
	//puts("density 3"); mat rho = sum(scale(dw, m), 1);
	puts("density 3"); mat rho = scale(dw, m);
	puts("density 4"); list_destroy(dxyz);
	printf("dw: %d, %d\n", dw->m, dw->n);
	//puts("density 5"); destroy(dw);
	puts("density 6"); return rho;
}

static mat
pressure(mat rho, double k, double n)
{
	mat p = scale(power(rho, 1 + 1 / n), k);
	return p;
}

static mat
acceleration(mat pos, mat vel, double m, double h,
	double k, double n, double l, double nu)
{
	puts("acceleration 0"); mat rho = density(pos, pos, m, h);
	mat p = pressure(rho, k, n);
	puts("acceleration 2"); matlist dxyz = pairwiseseparation(pos, pos);
	puts("acceleration 3"); matlist dwxyz = gradw(dxyz->x[0], dxyz->x[1],
		dxyz->x[2], h);
	mat ax = scale(sum(scale(multiply(add(divide(p,
		power(rho ,2)), divide(transpose(p), 
		power(transpose(rho), 2))), dwxyz->x[0]), m), 1), -1);
	mat ay = scale(sum(scale(multiply(add(divide(p, 
		power(rho, 2)), divide(transpose(p),
		power(transpose(rho), 2))), dwxyz->x[1]), m), 1), -1);
	mat az = scale(sum(scale(multiply(add(divide(p,
		power(rho, 2)), divide(transpose(p),
		power(transpose(rho), 2))), dwxyz->x[2]), m), 1), -1);
	mat acc = sub(hstack(hstack(ax, ay), az),
		add(scale(pos, l), scale(vel, nu)));
	destroy(rho);
	destroy(p);
	list_destroy(dxyz);
	list_destroy(dwxyz);
	destroy(ax);
	destroy(ay);
	destroy(az);
	return acc;
}

static void
sph_step(sph model)
{
	model->vel = add(model->vel, scale(model->acc, model->dt/2));
  model->pos = add(model->pos, scale(model->vel, model->dt));
	model->acc = acceleration(model->pos, model->vel, model->m,
		model->h, model->k, model->l, model->n, model->nu);
	model->vel = add(model->vel, scale(model->acc, model->dt/2));
	model->t += model->dt;
}

sph
sph_create(double dt, double h, double k, double n,
	double nu, double m, double pos[][3], int size)
{
	sph model = malloc(sizeof(sph));
	model->size = size;
	model->dt = dt;
	model->h = h;
	model->k = k;
	model->l = 2.01;
	model->n = n;
	model->nu = nu;
	model->m = m;
	model->pos = copy(3, pos, model->size);
	model->vel = create(size, 3);
	puts("sph_create"); model->acc = acceleration(model->pos, model->vel, model->m,
		model->h, model->k, model->l, model->n, model->nu);
	puts("sph_step"); model->step = &sph_step;
	return model;
}

void 
sph_destroy(sph model)
{
	free(model->vel);
	free(model->pos);
	free(model->acc);
	free(model);
}