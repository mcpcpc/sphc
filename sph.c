#include "sph.h"

static mat
w(mat dx, mat dy, mat dz, double h)
{
	double c = pow(1.0 / h * pow(M_PI, 1/2), 3);
	mat r = power(add(add(power(dx, 2), power(dy, 2)),
		power(dz, 2)), (1 / 2));
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
	wxyz->append(wxyz, multiply(n, x));
	wxyz->append(wxyz, multiply(n, y));
	wxyz->append(wxyz, multiply(n, z));
	destroy(r);
	destroy(n);
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
	matlist dxyz = pairwise_separation(r, pos);
	mat dw = w(dxyz->x[0], dxyz->x[1], dxyz->x[2], h);
	//mat rho = sum(scale(dw, m), 1);
	mat rho = scale(dw, m);
	list_destroy(dxyz);
	destroy(dw);
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
	mat rho = density(pos, pos, m, h);
	mat p = pressure(rho, k, n);
	matlist dxyz = pairwise_separation(pos, pos);
	matlist dwxyz = gradw(dxyz->x[0], dxyz->x[1],
		dxyz->x[2], h);
	mat ax = scale(sum(scale(multiply(add(divide(p,
		power(rho, 2)), divide(transpose(p),
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
	destroy(ax);
	destroy(ay);
	destroy(az);
	list_destroy(dxyz);
	list_destroy(dwxyz);
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

static void
sph_run(sph model, double t_final)
{
	while (model->t < t_final) {
		model->step(model);
	}
}

sph
sph_create(double dt, double h, double k, double n,
	double nu, double m, double pos[][3], int size)
{
	double lambda = 2.01;
	mat pos0 = copy(3, pos, size);
	mat vel0 = create(size, 3);
	mat acc0 = acceleration(pos0, vel0, m, h, k, lambda, n, nu);
	sph_t model = {
		.size = size,
		.dt = dt,
		.h = h,
		.k = k,
		.l = lambda,
		.n = n,
		.nu = nu,
		.m = m,
		.pos = pos0,
		.vel = vel0,
		.acc = acc0,
		.step = &sph_step,
		.run = &sph_run
	};
	sph ptr = malloc(sizeof(model));
	ptr->size = size;
	ptr->dt = dt;
	ptr->h = h;
	ptr->k = k;
	ptr->l = lambda;
	ptr->n = n;
	ptr->nu = nu;
	ptr->m = m;
	ptr->pos = pos0;
	ptr->vel = vel0;
	ptr->acc = acc0;
	ptr->step = &sph_step;
	ptr->run = &sph_run;
	return ptr;
}

void
sph_destroy(sph model)
{
	free(model);
}
