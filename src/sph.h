#ifndef __SPH_H
#define __SPH_H

#include "mat.h"

typedef struct SPH_MODEL {
    double dt;     // time step
    double t;      // current time
    double h;      // smoothing length
    double k;      // equation of state constant
    double n;      // polytropic index
    double nu;     // damping
    double l;      // lambda
    double m;      // paricle masees
    double r;      // particle radii
    int    size;   // number of particles
    mat    pos;
    mat    vel;
    mat    acc;
    void * (*init)(struct SPH_MODEL *self);
    void * (*step)(struct SPH_MODEL *self);
} sph_t, *sph;

void sph_init(sph m);
void sph_step(sph m);

#endif
