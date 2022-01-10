#include "sph.h"

static mat w(mat dx, mat dy, mat dz, double h) {
	mat r = power(add(add(power(dx,2),power(dy,2)),power(dz,2)),1/2);
	double c = pow(1.0 / h * pow(M_PI, 1/2), 3);
	mat w = scale(exponent(scale(power(r, 2),1/(h*h)),M_E),c);
	del(r);
	return w;
}

static matlist gradw(mat x, mat y, mat z, double h) {
	mat r = power(add(add(power(x,2),power(y,2)),power(z,2)),1/2);
	double c = -2 * pow(h, 5) / pow(M_PI, 3/2);
	mat n = scale(exponent(scale(scale(power(r,2),-1),1/h*h),M_E),c);
	matlist wxyz = list_alloc();
	wxyz->add(wxyz, multiply(n,x));
	wxyz->add(wxyz, multiply(n,y));
	wxyz->add(wxyz, multiply(n,z));
	del(r);
	del(n);
	return wxyz;
}

static matlist pairwiseseparation(mat ri, mat rj) {
	mat rix = submat(ri, 0, ri->m - 1, 0, 0);
	mat riy = submat(ri, 0, ri->m - 1, 1, 1);
	mat riz = submat(ri, 0, ri->m - 1, 2, 2);
	mat rjx = submat(rj, 0, rj->m - 1, 0, 0);
	mat rjy = submat(rj, 0, rj->m - 1, 1, 1);
	mat rjz = submat(rj, 0, rj->m - 1, 2, 2);
	matlist dxyz = list_alloc();
	dxyz->append(dxyz, sub(rix,transpose(rjx)));
	dxyz->append(dxyz, sub(riy,transpose(rjy)));
	dxyz->append(dxyz, sub(riz,transpose(rjz));
	del(rix);
	del(riy);
	del(riz);
	del(rjx);
	del(rjy);
	del(rjz);
	return dxyz;
}

static mat density(mat r, mat pos, double m, double h) {
	matlist dxyz = pairwiseseparation(r, pos);
	mat dw = w(dxyz->x[0], dxyz->x[0], dxyz->x[0], h);
	mat rho = sum(scale(dw, m), 1);
	del(dw);
	listdel(dxyz);
	return rho;
}

static mat pressure(mat rho, double k, double n) {
	mat p = scale(power(rho, 1 + 1 / n), k);
	return p;
}

static mat acceleration(mat pos, mat vel, double m, double h, double k, double n, double l, double nu) {
	mat rho = density(pos, pos, m, h);
	mat p = pressure(rho, k, n);
	matlist dxyz = pairwiseseparation(pos, pos);
	matlist dwxyz = gradw(dxyz->x[0], dxyz->x[1], dxyz->x[2], h);
	mat ax = scale(sum(scale(multiply(add(divide(p,power(rho,2)),divide(transpose(p),power(transpose(rho),2))),dwxyz->x[0]),m),1),-1);
	mat ay = scale(sum(scale(multiply(add(divide(p,power(rho,2)),divide(transpose(p),power(transpose(rho),2))),dwxyz->x[1]),m),1),-1);
	mat az = scale(sum(scale(multiply(add(divide(p,power(rho,2)),divide(transpose(p),power(transpose(rho),2))),dwxyz->x[2]),m),1),-1);
	mat acc = sub(hstack(hstack(ax, ay), az)),add(scale(pos,l),scale(vel,nu)));
	del(rho);
	del(p);
	list_del(dxyz);
	list_del(dwxyz);
	del(ax);
	del(ay);
	del(az);
	return acc;
}


static void sph_init(sph m) {
	m->vel = alloc(m->size, 3);
}

static void sph_step(sph m) {
	m->vel = add(m->vel, scale(m->acc, m->dt/2));
	m->pos = add(m->pos, scale(m->vel, m->dt));
	m->acc = acceleration(
		m->pos,
		m->vel,
		m->m,
		m->h,
		m->k,
		m->l,
		m->n,
		m->nu
	);
	m->vel = add(m->vel, scale(m->acc, m->dt/2));
	m->t += m->dt;
}

sph sph_alloc(double dt, double h, double k, double n, double nu, double m, double pos[][3], int size) {
	sph model = malloc(sizeof(sph));
	model->dt = dt;
	model->h = h;
	model->k = k;
	model->l = 2.01;
	model->n = n;
	model->nu = nu;
	model->m = m;
	model->pos = copy(3, pos, size);
	model->init = &sph_init;
	model->step = &sph_step;
	return model;
}