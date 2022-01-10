#ifndef __SPH_H
#define __SPH_H

#include "mat.h"
#include "list.h"

typedef struct SPH_MODEL {
    double dt;     // time step
    double t;      // current time
    double h;      // smoothing length
    double k;      // equation of state constant
    double n;      // polytropic index
    double nu;     // damping
    double l;      // lambda
    double m;      // particle masses
    int    size;   // number of particles
    mat    pos;
    mat    vel;
    mat    acc;
    void (*init)(struct SPH_MODEL *self);
    void (*step)(struct SPH_MODEL *self);
} sph_t, *sph;

sph sph_alloc(
	double dt,
	double h,
	double k,
	double n,
	double nu,
	double m,
	double pos[][3],
	int size
);

#endif
