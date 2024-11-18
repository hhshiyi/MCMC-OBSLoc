#include "inversion.h"

PDF likelihood(CONF conf, double sx, double sy, double sz, double st, STATION *stns, EVENT evt)
{
    double sigma_t = conf.sigma_t;
    double obs_t, cal_t, sum_p, sum_t;
    PDF result;

    sum_t = 0.0;
    sum_p = 0.0;
    for(int i = 0; i < evt.nump + evt.nums; i++)
        for(int j = 0; j < conf.nstn; j++)
            if(evt.arrivals[i].nstn == stns[j].id)
            {
                cal_t = forward(stns[j].x, stns[j].y, stns[j].z, sx, sy, sz);
                obs_t = evt.arrivals[i].tt + st;
                sum_p = sum_p + (cal_t - obs_t) * (cal_t - obs_t) / (sigma_t * sigma_t);
                sum_t = sum_t + (cal_t - obs_t) * (cal_t - obs_t);
            }
    result.pdf = exp(-0.5 * sum_p);
    result.rms = sqrt(sum_t / (evt.nump + evt.nums));
    //fprintf(stdout, "Sigma: %lf\n", result.pdf);
    return result;
}

// Metropolis sampling algorithm
void Metropolis(CONF conf, EVENT evt, STATION *stns, POINT **bath, FILE *fp)
{
    PDF result_current, result_new;
    double delta_x = conf.delta_x;
    double delta_y = conf.delta_y;
    double delta_z = conf.delta_z;
    double delta_t = conf.delta_t;
    double start_x, start_y, start_z, start_t;
    double prob_ac, current_sigma_model, new_sigma_model, **samples;

    samples = (double**)calloc(6, sizeof(double*));
    for(int j = 0; j < 6; j++)
    {
        samples[j] = (double*)calloc(conf.nsamples, sizeof(double));
    }

    if(conf.ifix == 1) getBathymetry(conf, bath);

    start_t = 0.0;
    if(conf.isyn)
    {
        start_x = (double)rand() / (double)RAND_MAX * (conf.max_x - conf.min_x) + conf.min_x;
        start_y = (double)rand() / (double)RAND_MAX * (conf.max_y - conf.min_y) + conf.min_y;
        if(conf.ifix == 1) start_z = interpolation2D(start_x, start_y, bath, conf);
        else start_z = (double)rand() / (double)RAND_MAX * (conf.max_z - conf.min_z) + conf.min_z;
    }
    else
    {
        start_x = evt.x;
        start_y = evt.y;
        if(conf.ifix == 1) start_z = interpolation2D(start_x, start_y, bath, conf);
        else start_z = evt.z;
    }

    for(int j = 0, k = 0; k < conf.nsamples; j++)
    {
        result_current = likelihood(conf, start_x, start_y, start_z, start_t, stns, evt);
        current_sigma_model = result_current.pdf;

        double st = start_t + 2.0 * delta_t * ((double)rand() / (double)RAND_MAX - 0.5);
        double sx = start_x + 2.0 * delta_x * ((double)rand() / (double)RAND_MAX - 0.5);
        double sy = start_y + 2.0 * delta_y * ((double)rand() / (double)RAND_MAX - 0.5);

        if(sx < conf.min_x || sx > conf.max_x || \
           sy < conf.min_y || sy > conf.max_y)
            continue;

        double sz = start_z + 2.0 * delta_z * ((double)rand() / (double)RAND_MAX - 0.5);
        if(conf.ifix == 1) sz = interpolation2D(sx, sy, bath, conf);

        if(sz < conf.min_z || sz > conf.max_z)
            continue;

        result_new = likelihood(conf, sx, sy, sz, st, stns, evt);
        new_sigma_model = result_new.pdf;

        if(current_sigma_model == 0.0) prob_ac = 1.0;
        else prob_ac = new_sigma_model / current_sigma_model;

        if(prob_ac >= 1.0)
        {
            start_x = sx;
            start_y = sy;
            start_z = sz;
            start_t = st;
            samples[0][k] = sx;
            samples[1][k] = sy;
            samples[2][k] = sz;
            samples[3][k] = st;
            samples[4][k] = new_sigma_model;
            samples[5][k] = result_new.rms;
            k++;
            fprintf(stdout, "%d\n", k);
        }
        else
        {
            // generate a random number in [0,1]-->probability
            double prob = (double)rand() / (double)RAND_MAX;
            if(prob_ac > prob)
            {
                start_x = sx;
                start_y = sy;
                start_z = sz;
                start_t = st;
                samples[0][k] = sx;
                samples[1][k] = sy;
                samples[2][k] = sz;
                samples[3][k] = st;
                samples[4][k] = new_sigma_model;
                samples[5][k] = result_new.rms;
                k++;
                fprintf(stdout, "%d\n", k);
            }
        }
    }

    // output samples and statistic values
    statistic(samples, conf, fp);
}

void statistic(double **samples, CONF conf, FILE *fp)
{
    int max_likelihood = -12345;
    double max_pdf = samples[4][conf.nsamples - conf.savesamples];
    double lon, lat, dep;

    double ex = 0.0, ey = 0.0, ez = 0.0, et = 0.0;
    double dx = 0.0, dy = 0.0, dz = 0.0, dt = 0.0;

    for(int j = 0; /*conf.nsamples - conf.savesamples;*/ j < conf.nsamples; j++)
    {
        if(j >= conf.nsamples - conf.savesamples)
        {
            if(samples[4][j] > max_pdf)
            {
                max_pdf = samples[4][j];
                max_likelihood = j;
            }

            ex = ex + samples[0][j];
            ey = ey + samples[1][j];
            ez = ez + samples[2][j];
            et = et + samples[3][j];
        }

        xyz2geo(conf, samples[0][j], samples[1][j], samples[2][j], &lon, &lat, &dep);
        fprintf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf\n",samples[0][j],samples[1][j],samples[2][j],samples[3][j],samples[4][j],samples[5][j],lon,lat,dep);
    }

    ex = ex / conf.savesamples;
    ey = ey / conf.savesamples;
    ez = ez / conf.savesamples;
    et = et / conf.savesamples;

    xyz2geo(conf, ex, ey, ez, &lon, &lat, &dep);
    fprintf(fp, "# %lf %lf %lf %lf %lf %lf %lf #statistical results\n",ex, ey, ez, et,lon,lat,dep);

    xyz2geo(conf, samples[0][max_likelihood], samples[1][max_likelihood], samples[2][max_likelihood],\
                  &lon, &lat, &dep);
    fprintf(fp, "# %lf %lf %lf %lf %lf %lf %lf %lf %lf #maximum likelihood results\n",\
            samples[0][max_likelihood], samples[1][max_likelihood], samples[2][max_likelihood],\
            samples[3][max_likelihood], samples[4][max_likelihood], samples[5][max_likelihood],\
            lon, lat, dep);

    for(int j = conf.nsamples - conf.savesamples; j < conf.nsamples; j++)
    {
        dx = dx + (samples[0][j] - ex) * (samples[0][j] - ex);
        dy = dy + (samples[1][j] - ey) * (samples[1][j] - ey);
        dz = dz + (samples[2][j] - ez) * (samples[2][j] - ez);
        dt = dt + (samples[3][j] - et) * (samples[3][j] - et);
    }

    dx = dx / conf.savesamples;
    dy = dy / conf.savesamples;
    dz = dz / conf.savesamples;
    dt = dt / conf.savesamples;

    dx = sqrt(dx);
    dy = sqrt(dy);
    dz = sqrt(dz);
    dt = sqrt(dt);

    fprintf(fp, "# %lf %lf %lf %lf #standard deviations\n",dx, dy, dz, dt);
}

void Gibbs(CONF conf, EVENT evt, STATION *stns, POINT **bath, FILE *fp)
{
    PDF result_old, result_new;
    double delta_x = conf.delta_x;
    double delta_y = conf.delta_y;
    double delta_z = conf.delta_z;
    double delta_t = conf.delta_t;
    double old_x, old_y, old_z, old_t;
    double old_sigma_model, new_sigma_model, old_rms, new_rms;
    double acceptance_ratio, **samples;

    samples = (double**)calloc(6, sizeof(double*));
    for(int j = 0; j < 6; j++)
    {
        samples[j] = (double*)calloc(conf.nsamples, sizeof(double));
    }

    if(conf.ifix == 1) getBathymetry(conf, bath);

    // initialize variations
    old_t = 0.0;
    old_x = (double)rand() / (double)RAND_MAX * (conf.max_x - conf.min_x) + conf.min_x;
    old_y = (double)rand() / (double)RAND_MAX * (conf.max_y - conf.min_y) + conf.min_y;
    if(conf.ifix == 1) old_z = interpolation2D(old_x, old_y, bath, conf);
    else old_z = (double)rand() / (double)RAND_MAX * (conf.max_z - conf.min_z) + conf.min_z;

    for(int j = 0, k = 0; k < conf.nsamples; j++)
    {
        // sample t, while x, y and z are fixed
        result_old = likelihood(conf, old_x, old_y, old_z, old_t, stns, evt);
        old_sigma_model = result_old.pdf;
        old_rms = result_old.rms;

        while (1)
        {
            double new_t = old_t + 2.0 * delta_t * ((double)rand() / (double)RAND_MAX - 0.5); // 生成候选点

            result_new = likelihood(conf, old_x, old_y, old_z, new_t, stns, evt);
            new_sigma_model = result_new.pdf;
            new_rms = result_new.rms;

            if(old_sigma_model == 0.0) acceptance_ratio = 1.0;
            else acceptance_ratio = new_sigma_model / old_sigma_model;

            if((double)rand() / (double)RAND_MAX < acceptance_ratio)
            {
                old_t = new_t;
                old_sigma_model = new_sigma_model;
                old_rms = new_rms;
                break;
            }
        }

        // sample x, while y, z and t are fixed
        while (1)
        {
            double new_x = old_x + 2.0 * delta_x * ((double)rand() / (double)RAND_MAX - 0.5); // 生成候选点
            if(conf.ifix == 1)
            {
                if(new_x < conf.min_x || new_x > conf.max_x)
                    continue;
                else
                    old_z = interpolation2D(new_x, old_y, bath, conf);
            }

            result_new = likelihood(conf, new_x, old_y, old_z, old_t, stns, evt);
            new_sigma_model = result_new.pdf;
            new_rms = result_new.rms;

            if(old_sigma_model == 0.0) acceptance_ratio = 1.0;
            else acceptance_ratio = new_sigma_model / old_sigma_model;

            if((double)rand() / (double)RAND_MAX < acceptance_ratio)
            {
                old_x = new_x;
                old_sigma_model = new_sigma_model;
                old_rms = new_rms;
                break;
            }
        }

        // sample y, while x, z and t are fixed
        while (1)
        {
            double new_y = old_y + 2.0 * delta_y * ((double)rand() / (double)RAND_MAX - 0.5); // 生成候选点
            if(conf.ifix == 1)
            {
                if(new_y < conf.min_y || new_y > conf.max_y)
                    continue;
                else
                    old_z = interpolation2D(old_x, new_y, bath, conf);
            }

            result_new = likelihood(conf, old_x, new_y, old_z, old_t, stns, evt);
            new_sigma_model = result_new.pdf;
            new_rms = result_new.rms;

            if(old_sigma_model == 0.0) acceptance_ratio = 1.0;
            else acceptance_ratio = new_sigma_model / old_sigma_model;

            if((double)rand() / (double)RAND_MAX < acceptance_ratio)
            {
                old_y = new_y;
                old_sigma_model = new_sigma_model;
                old_rms = new_rms;
                break;
            }
        }

        if(conf.ifix == 1)
        {
            samples[0][k] = old_x;
            samples[1][k] = old_y;
            samples[2][k] = old_z;
            samples[3][k] = old_t;
            samples[4][k] = old_sigma_model;
            samples[5][k] = old_rms;
            k++;
        }
        else
        {
            // sample z, while x, y and t are fixed
            while (1)
            {
                double new_z = old_z + 2.0 * delta_z * ((double)rand() / (double)RAND_MAX - 0.5); // 生成候选点

                result_new = likelihood(conf, old_x, old_y, new_z, old_t, stns, evt);
                new_sigma_model = result_new.pdf;
                new_rms = result_new.rms;

                if(old_sigma_model == 0.0) acceptance_ratio = 1.0;
                else acceptance_ratio = new_sigma_model / old_sigma_model;

                if((double)rand() / (double)RAND_MAX < acceptance_ratio)
                {
                    old_z = new_z;
                    old_sigma_model = new_sigma_model;
                    old_rms = new_rms;
                    break;
                }
            }

            samples[0][k] = old_x;
            samples[1][k] = old_y;
            samples[2][k] = old_z;
            samples[3][k] = old_t;
            samples[4][k] = old_sigma_model;
            samples[5][k] = old_rms;
            k++;
        }
    }

    // output samples and set samples zeros
    statistic(samples, conf, fp);
}
