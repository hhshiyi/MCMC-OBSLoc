#include <stdio.h>
#include <data_io.h>
#include <inversion.h>

int main(int argc, char *argv[])
{
    CONF conf;
    EVENT *evts = NULL;
    STATION *stns = NULL;
    FILE *fid = NULL;
    POINT **bathy = NULL;

    // step 1: read configurations from CONFIG file
    conf = initializing();
    //dispConf(conf);

    // step 2: read station list
    stns = (STATION*)calloc(conf.nstn, sizeof(STATION));
    getStations(stns, conf);

    // step 3: read event list
    evts = (EVENT*)calloc(conf.neqs, sizeof(EVENT));
    getEvents(evts, conf);

    // test forward
    if(conf.isyn) synthetics(evts, stns, conf);

    // step 4: allocate memory for bathymetric data
    if(conf.ifix == 1)
    {
        bathy = (POINT**)calloc(conf.bx, sizeof(POINT*));
        for(int i = 0; i < conf.bx; i++)
        {
            bathy[i] = (POINT*)calloc(conf.by, sizeof(POINT));
        }
    }

    // step 5: do Monte Carlo inversion
    srand(conf.seed);
    for(int i = 0; i < conf.neqs; i++)
    {
        char out_loc[128];
        sprintf(out_loc, "result/%s_%05d.loc", evts[i].phase, evts[i].id);
        fid = fopen(out_loc, "w");
        if(conf.algorithm == 0)
            Metropolis(conf, evts[i], stns, bathy, fid);
        else
            Gibbs(conf, evts[i], stns, bathy, fid);

        fclose(fid);
    }

    return 1;
}


