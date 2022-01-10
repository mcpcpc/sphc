#include "sph.h"
#include <stdio.h>

static mat
w(mat dx, mat dy, mat dz, double h)
{
	puts("w 1"); double c = pow(1.0 / h * pow(M_PI, 1/2), 3);
	puts("w 2"); mat r = power(add(add(power(dx, 2), power(dy, 2)), 
		power(dz, 2)), (1 / 2));
	puts("w 3"); mat w = scale(exponent(scale(power(r, 2),
		1 / (h * h)), M_E), c);
	puts("w 4"); destroy(r);
	return w;
}

static matlist
gradw(mat x, mat y, mat z, double h)
{
	puts("gradw 1"); mat r = power(add(add(power(x ,2), power(y, 2)),
		power(z, 2)), (1 / 2));
	puts("gradw 2"); double c = -2 * pow(h, 5) / pow(M_PI, (3 / 2));
	puts("gradw 3"); mat n = scale(exponent(scale(scale(power(r, 2), -1),
		1 / h * h ), M_E), c);
	puts("gradw 4"); matlist wxyz = list_create();
	puts("gradw 5"); wxyz->append(wxyz, multiply(n, x));
	puts("gradw 6"); wxyz->append(wxyz, multiply(n, y));
	puts("gradw 7"); wxyz->append(wxyz, multiply(n, z));
	puts("gradw 8"); destroy(r);
	puts("gradw 9"); destroy(n);
	return wxyz;
}

static matlist
pairwise_separation(mat ri, mat rj)
{
	mat rix = submat(ri, 0, ri->m - 1, 0, 0);
	mat riy = submat(ri, 0, ri->m - 1, 1, 1);
	mat riz = submat(ri, 0, ri->m - 1, 2, 2);
	mat rjx = submat(rj, 0, rj->m - 1, 0, 0);
	mat rjy = submat(rj, 0, rj->m - 1, 1, 1);
	mat rjz = submat(rj, 0, rj->m - 1, 2, 2);
	matlist dxyz = list_create();
	dxyz->append(dxyz, distance(rix,transpose(rjx)));
	dxyz->append(dxyz, distance(riy,transpose(rjy)));
	dxyz->append(dxyz, distance(riz,transpose(rjz)));
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
	puts("density 0"); matlist dxyz = pairwise_separation(r, pos);
	puts("density 1"); mat dw = w(dxyz->x[0], dxyz->x[1], dxyz->x[2], h);
	//mat rho = sum(scale(dw, m), 1);
	puts("density 2"); mat rho = scale(dw, m);
	puts("density 3"); list_destroy(dxyz);
	puts("density 4"); destroy(dw);
	return rho;
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
	puts("acceleration 1"); mat p = pressure(rho, k, n);
	puts("acceleration 2"); matlist dxyz = pairwise_separation(pos, pos);
	puts("acceleration 3"); matlist dwxyz = gradw(dxyz->x[0], dxyz->x[1],
		dxyz->x[2], h);
	puts("acceleration 4"); mat ax = scale(sum(scale(multiply(add(divide(p,
		power(rho, 2)), divide(transpose(p), 
		power(transpose(rho), 2))), dwxyz->x[0]), m), 1), -1);
	puts("acceleration 5"); mat ay = scale(sum(scale(multiply(add(divide(p, 
		power(rho, 2)), divide(transpose(p),
		power(transpose(rho), 2))), dwxyz->x[1]), m), 1), -1);
	puts("acceleration 6"); mat az = scale(sum(scale(multiply(add(divide(p,
		power(rho, 2)), divide(transpose(p),
		power(transpose(rho), 2))), dwxyz->x[2]), m), 1), -1);
	puts("acceleration 7"); mat acc = sub(hstack(hstack(ax, ay), az),
		add(scale(pos, l), scale(vel, nu)));
	puts("acceleration 8"); destroy(rho);
	puts("acceleration 9"); destroy(p);
	puts("acceleration 10"); list_destroy(dxyz);
	puts("acceleration 11"); list_destroy(dwxyz);
	puts("acceleration 12"); destroy(ax);
	puts("acceleration 13"); destroy(ay);
	puts("acceleration 14"); destroy(az);
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