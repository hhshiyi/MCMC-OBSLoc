#ifndef INVERSION_H
#define INVERSION_H

#include <model.h>
#include <forward.h>
#include <data_io.h>
#include <geo2xyz.h>
#include <interpolation.h>

typedef struct result_likelihood
{
    double pdf;
    double rms;
} PDF;

void Metropolis(CONF conf, EVENT evt, STATION *stns, POINT **bath, FILE *fp);
void Gibbs(CONF conf, EVENT evt, STATION *stns, POINT **bath, FILE *fp);
void statistic(double **samples, CONF conf, FILE *fp);
PDF likelihood(CONF conf, double sx, double sy, double sz, double st, STATION *stns, EVENT evt);

#endif // INVERSION_H
