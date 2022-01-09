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
    double lambda; // 
    double mass;   // paricle masees
    double radius; // particle radii
    int    count;  // number of particles
    mat    position;
    mat    velocity;
    mat    acceleration;
    void * (*init)(struct SPH_MODEL *self);
    void * (*step)(struct SPH_MODEL *self);
} sph_t;

void init(sph_t m);
void step(sph_t m);

#endif
