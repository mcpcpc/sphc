#include "sim.h"

//#define LUMIN_C ".:;=!*#@"

static void
sim_dump(mat out, int height, int width)
{
	printf("\x1b[H");
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			putchar(out->v[i,j]);
		}
	putchar('\n');
	}
}

static void
flatland(double x, double y, double z, double u, double v)
{
	u = x / y;
	v = y / z;
}

static void
render(mat x, mat y, double mscale, double nscale)
{
	double u = 0, v = 0;
	int ui = 0, vi = 0; 
	for(int i = 0; i < mat->m; i++) {
			flatland(x->v[i][0], x->v[i][1], x->v[i][2], u, v);
			ui = (int)(u * mscale);
			vi = (int)(v * nscale);
			y->v[ui][vi] += 1; 
	}
}

sim
sim_init(sph model, double m1, double m2, double n1, double n2)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	sim_t simulation = {
		.model = model,
		.height = (double)size.ws_col,
		.width = (double)size.ws_row,
		.m_lim = {m1,m2},
		.n_lim = {n1,n2},
		.mscale =  (double)size.ws_col / (m2 - m1),
		.nscale =  (double)size.ws_row / (n2 - n1),
		.out = create(model->m, model->n - 1)
	};
	return simulation;
}

void 
sim_run(sim simulation)
{
	while(1) {
			simulation->model.step(simulation->model);
			render(simulation->model->pos, simulation->out, simulation->mscale, simulation->nscale);
			sim_dump(simulation->out, simulation->height, simulation->width);
	}
}
