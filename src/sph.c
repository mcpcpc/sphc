#include "sph.h"
/*
mat w(mat dx, mat dy, mat dz, double h) {
	mat r = power(add(add(power(dx,2),power(dy,2)),power(dz,2)),1/2);
	double c = pow(1.0 / h * pow(M_PI, 1/2), 3);
	mat w = scale(exponent(div(power(r, 2),(h*h)),M_E),c);
	del(r);
	return w;
}

mat gradw(mat x, mat y, mat z, double h) {
	mat r = power(add(add(power(x,2),power(y,2)),power(z,2)),1/2);
	double c = -2 * pow(h, 5) / pow(M_PI, 3/2);
	mat n = scale(exponent(scale(scale(power(r,2),-1),1/h*h),M_E),c);
	mat wxyz[3] = {multiply(n,x), multiply(n,y), multiply(n,z)}
	del(r);
	del(n);
	return wxyz;
}

mat pairwiseseparation(mat ri, mat rj) {
	mat rix = submat(ri, 0, ri->m - 1, 0, 0);
	mat riy = submat(ri, 0, ri->m - 1, 1, 1);
	mat riz = submat(ri, 0, ri->m - 1, 2, 2);
	mat rjx = submat(rj, 0, rj->m - 1, 0, 0);
	mat rjy = submat(rj, 0, rj->m - 1, 1, 1);
	mat rjz = submat(rj, 0, rj->m - 1, 2, 2);
	mat dx = sub(rix,transpose(rjx));
	mat dy = sub(riy,transpose(rjy));
	mat dz = sub(riz,transpose(rjz));
	del(rix);
	del(riy);
	del(riz);
	del(rjx);
	del(rjy);
	del(rjz);
	del(dx);
	del(dy);
	del(dz);
	return dxyz;
}

mat density(mat r, mat pos, double m, double h) {
	mat dxyz = pairwiseseparation(r, pos);
	mat rho = ;
	del(dxyz);
	return rho;
}
*/
mat pressure(mat rho, double k, double n) {
	mat p = scale(power(rho, 1 + 1 / n), k);
	return p;
}
/*
mat acceleration(mat pos, mat vel, double m, double h, double k, double l, double nu) {
	mat rho = density(pos, pos, m, h);
	mat pressure = pressure(rho, k, n);
	mat dxyz = pairwiseseparation(pos, pos);
	mat dwxyz = gradw(dxyz, h);
	mat acc =;
	
	del(rho);
	del(pressure);
	del(dxyz);
	del(dwxyz);
	del(ax);
	del(ay);
	del(az);
	return acc;
}
*/
void init(sph m) {
	m->vel = alloc(m->size, 3);
}

void step(sph m) {
	//m->vel = add(m->vel, scale(m->acc, dt/2));
	//m->pos = add(m->pos, scale(m->vel, dt));
	//m->acc = acceleration(
	//	m->pos,
	//	m->vel,
	//	m->m,
	//	m->h,
	//	m->k,
	//	m->l,
	//	m->nu
	//);
	//m->vel = add(m->vel, scale(m->acc, dt/2));
	//m->t += m->dt;
}