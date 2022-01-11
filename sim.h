#ifndef __SPH_H
#define __SPH_H

#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include "sph.h"

typedef struct SIMULATOR {
	sph    model;
	int    width;
	int    height;
	double m_lim[2];
	double n_lim[2];
	double mscale;
	double nscale;
	mat    out;
} sim_t, *sim;

void sim_init(sph model, double m1, double m2, double n1, double n2);
void sim_run(sim simulation);
//void sim_finish(sph model);

#endif