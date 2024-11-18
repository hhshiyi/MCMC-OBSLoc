#include "forward.h"

// calculate travel time from source to receiver in Cartesian coordinates
double forward(double rx, double ry, double rz, double sx, double sy, double sz)
{
    double dist;
    dist = distance(rx, ry, rz, sx, sy, sz);
    return dist / 1.50;
}

double distance(double rx, double ry, double rz, double sx, double sy, double sz)
{
    return sqrt((sx - rx) * (sx - rx) + (sy - ry) * (sy - ry) + (sz - rz) * (sz - rz));
}

void synthetics(EVENT *evts, STATION *stns, CONF conf)
{
    FILE *fid = NULL;

    if((fid = fopen("syn/syn.d", "w")) == NULL)
    {
        fprintf(stderr, "Can't create file %s\n", "syn/syn.d");
        exit(1);
    }

    for(int i = 0; i < conf.neqs; i++)
    {
        fprintf(fid, "%d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %d %d %s %d\n",\
                evts[i].origin.year,   evts[i].origin.month, evts[i].origin.day,  evts[i].origin.hour,\
                evts[i].origin.minute, evts[i].origin.second,evts[i].dsec,\
                evts[i].elat,  evts[i].dlat, evts[i].elon, evts[i].dlon,   evts[i].depth,\
                evts[i].ddepth, evts[i].mag,   evts[i].nump, evts[i].nums, evts[i].phase,  evts[i].id);

        for(int j = 0; j < evts[i].nump + evts[i].nums; j++)
            for(int k = 0; k < conf.nstn; k++)
                if(evts[i].arrivals[j].nstn == stns[k].id)
                {
                    double cal_t = forward(stns[k].x, stns[k].y, stns[k].z, evts[i].x, evts[i].y, evts[i].z);
                    double noise = gaussian_noise(conf);
                    if(conf.inoise) cal_t = cal_t + noise;
                    fprintf(fid, "%d %lf %d\n", evts[i].arrivals[j].nstn, cal_t, evts[i].arrivals[j].ikps);
                }
    }
    fclose(fid);
}

double gaussian_noise(CONF conf)
{
    double noise = normal(0.0, conf.stddev_noise);
    return noise;
}

// Box-Muller algorithm to generate Gaussian random number
// mu: mean
// sigma: standard deviation
// return a random number falling in -3*sigma to 3*sigma centered around mu
double normal(double mu, double sigma)
{
    static double z1, z2;
    static int generate;

    generate = !generate;

    if(!generate) return z2 * sigma + mu;

    double u1, u2;

    do{
        u1 = (double)rand() / (double)RAND_MAX; //rand() * (1.0 / RAND_MAX);
        u2 = (double)rand() / (double)RAND_MAX; //rand() * (1.0 / RAND_MAX);
    } while (u1 <= 1e-7);

    z1 = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.141592654 * u2);
    z2 = sqrt(-2.0 * log(u1)) * sin(2.0 * 3.141592654 * u2);

    return z1 * sigma + mu;
}
