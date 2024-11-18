#include <string.h>
#include <stdlib.h>
#include <math.h>

//#include <model.h>
#include <data_io.h>
#include <geo2xyz.h>

CONF initializing()
{
    FILE *fp = NULL;
    CONF conf;

    if((fp = fopen("CONFIG", "r")) == NULL)
    {
        fprintf(stderr, "Can't find file %s\n", "CONFIG");
        exit(1);
    }

    sscanf(parseLine(fp, 45), "%d", &conf.verb);
    if(conf.verb == 1) info();

    conf.refTime = str2Time(parseLine(fp, 45));
    if(conf.verb == 1)
    {
        fprintf(stdout, "%-50s", "Reference origin time:");
        dispTime(conf.refTime);
    }

    sscanf(parseLine(fp, 45), "%d", &conf.isyn);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d\n", "Synthetic test:", conf.isyn);

    sscanf(parseLine(fp, 45), "%d %lf", &conf.inoise, &conf.stddev_noise);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d %lf\n", "Gaussian Noise, standard deviation:", \
                               conf.inoise, conf.stddev_noise);

    sscanf(parseLine(fp, 45), "%d", &conf.iout_loc);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d\n", "Output relocations:", conf.iout_loc);

    sscanf(parseLine(fp, 45), "%d %lf", &conf.ishotcorr, &conf.vlen);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d %lf\n", "length of research vessel:", conf.ishotcorr, conf.vlen);

    sscanf(parseLine(fp, 45), "%d", &conf.nstn);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d\n", "The number of stations:", conf.nstn);

    sscanf(parseLine(fp, 45), "%s", conf.file_stn);
    if(conf.verb == 1) fprintf(stdout, "%-50s%s\n", "The file name of station list:", conf.file_stn);

    sscanf(parseLine(fp, 45), "%d", &conf.neqs);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d\n", "Number of earthquakes:", conf.neqs);

    sscanf(parseLine(fp, 45), "%s", conf.file_pick);
    if(conf.verb == 1) fprintf(stdout, "%-50s%s\n", "The file name of earthquake list:", conf.file_pick);

    sscanf(parseLine(fp, 45), "%d", &conf.ifix);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d\n", "If seafloor fixed:", conf.ifix);

    sscanf(parseLine(fp, 45), "%d %d %lf", &conf.bx, &conf.by, &conf.grid_interval);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d %d %lf\n", "grid size of bathymetric data:", conf.bx, conf.by, conf.grid_interval);

    sscanf(parseLine(fp, 45), "%s", conf.file_bath);
    if(conf.verb == 1) fprintf(stdout, "%-50s%s\n", "Bathymetry file of the searching area:", conf.file_bath);

    sscanf(parseLine(fp, 45), "%lf %lf %lf", &conf.origLon, &conf.origLat, &conf.rotAng);
    if(conf.verb == 1) fprintf(stdout, "%-50s%lf %lf %lf\n", "Reference point and rotate angle:", conf.origLon, conf.origLat, conf.rotAng);

    sscanf(parseLine(fp, 45), "%lf %lf %lf %lf %lf %lf", &conf.min_x, &conf.max_x, &conf.min_y, &conf.max_y, &conf.min_z, &conf.max_z);
    if(conf.verb == 1) fprintf(stdout, "%-50s%lf %lf %lf %lf %lf %lf\n", "Model ranges:", conf.min_x, conf.max_x, conf.min_y, conf.max_y, conf.min_z, conf.max_z);

    sscanf(parseLine(fp, 45), "%d %d", &conf.seed, &conf.algorithm);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d %d\n", "Random seed and sampling method:", conf.seed, conf.algorithm);

    sscanf(parseLine(fp, 45), "%d %d", &conf.nsamples, &conf.savesamples);
    if(conf.verb == 1) fprintf(stdout, "%-50s%d %d\n", "Samples of Monte Carlo method:", conf.nsamples, conf.savesamples);

    sscanf(parseLine(fp, 45), "%lf %lf %lf %lf %lf", &conf.delta_x, &conf.delta_y, &conf.delta_z, &conf.delta_t, &conf.sigma_t);
    if(conf.verb == 1) fprintf(stdout, "%-50s%lf %lf %lf %lf %lf\n", "Temporal and spatial uncertainties:", conf.delta_x, conf.delta_y, conf.delta_z, conf.delta_t, conf.sigma_t);

    fclose(fp);

    return conf;
}

void dispConf(CONF conf)
{
    info();
    fprintf(stdout, "%-50s", "Reference origin time:");
    dispTime(conf.refTime);
    //fprintf(stdout, "%-50s%d\n", "Cartesian or Spherical coordinates:", conf.icoordinate);
    fprintf(stdout, "%-50s%d\n", "Synthetic test:", conf.isyn);
    fprintf(stdout, "%-50s%d\n", "Output relocations:", conf.iout_loc);
    fprintf(stdout, "%-50s%d\n", "The number of stations:", conf.nstn);
    fprintf(stdout, "%-50s%s\n", "The file name of station list:", conf.file_stn);
    fprintf(stdout, "%-50s%d\n", "Number of earthquakes:", conf.neqs);
    fprintf(stdout, "%-50s%s\n", "The file name of earthquake list:", conf.file_pick);
    fprintf(stdout, "%-50s%s\n", "Bathymetry file of the searching area:", conf.file_bath);
    fprintf(stdout, "%-50s%lf %lf %lf\n", "Reference point and rotate angle:", conf.origLon, conf.origLat, conf.rotAng);
    fprintf(stdout, "%-50s%lf %lf %lf %lf %lf %lf\n", "Model ranges:", conf.min_x, conf.max_x, conf.min_y, conf.max_y, conf.min_z, conf.max_z);
    fprintf(stdout, "%-50s%d\n", "Random seed:", conf.seed);
    fprintf(stdout, "%-50s%d %d\n", "Samples of Monte Carlo method:", conf.nsamples, conf.savesamples);
    //fprintf(stdout, "%-50s%lf %lf %lf %lf\n", "Temporal and spatial uncertainties:", conf.sigma_x, conf.sigma_y, conf.sigma_z, conf.sigma_t);
}

void getStations(STATION *stns, CONF conf)
{
    double tmp;
    FILE *fp = NULL, *fpout = NULL;
    if((fp = fopen(conf.file_stn, "r")) == NULL)
    {
        fprintf(stderr, "Can't find file %s\n", conf.file_stn);
        exit(1);
    }

    if((fpout = fopen("result/station.d", "w")) == NULL)
    {
        fprintf(stderr, "Can't write file %s\n", "result/station.d");
        exit(1);
    }

    if(conf.verb) fprintf(stdout, "\nStation List:\n");
    for(int i = 0; i < conf.nstn; i++)
    {
        if(conf.ishotcorr)
            fscanf(fp, "%d %s %lf %lf %lf %lf", &stns[i].id, stns[i].stnm, &stns[i].slat, &stns[i].slon, &stns[i].sele, &stns[i].ori);
        else
            fscanf(fp, "%d %s %lf %lf %lf %lf", &stns[i].id, stns[i].stnm, &stns[i].slat, &stns[i].slon, &stns[i].sele, &tmp);

        stns[i].sele *= -1.0;

        geo2xyz(conf, stns[i].slon, stns[i].slat,stns[i].sele, &stns[i].x, &stns[i].y, &stns[i].z);

        if(conf.ishotcorr) shotcorrect(stns[i].x, stns[i].y, stns[i].ori, conf, &stns[i].x, &stns[i].y);

        if(stns[i].x < conf.min_x || stns[i].x > conf.max_x ||\
           stns[i].y < conf.min_y || stns[i].y > conf.max_y ||\
           stns[i].z < conf.min_z || stns[i].z > conf.max_z)
        {
            fprintf(stdout,"Station out of model: %d %s %lf %lf %lf\n", stns[i].id, stns[i].stnm, stns[i].slat, stns[i].slon, stns[i].sele);
            exit(1);
        }

        fprintf(fpout, "%d %s %lf %lf %lf\n", stns[i].id, stns[i].stnm, stns[i].x, stns[i].y, stns[i].z);

        if(conf.verb)
            fprintf(stdout,"%d %s %lf %lf %lf\n", stns[i].id, stns[i].stnm, stns[i].slat, stns[i].slon, stns[i].sele);
    }
    fclose(fp);
    fclose(fpout);
}

void shotcorrect(double x, double y, double ori, CONF conf, double *xc, double *yc)
{
    double dx, dy;
    dx = conf.vlen / 1000.0 * sin(deg2rad(ori));
    dy = conf.vlen / 1000.0 * cos(deg2rad(ori));

    *xc = x - dx;
    *yc = y - dy;
}

void getEvents(EVENT *evts, CONF conf)
{
    FILE *fid = NULL;
    if((fid = fopen(conf.file_pick, "r")) == NULL)
    {
        fprintf(stderr, "Can't find file %s\n", conf.file_pick);
        exit(1);
    }

    if(conf.verb) fprintf(stdout, "\nEvent List:\n");

    for(int i = 0; i < conf.neqs; i++)
    {
        fscanf(fid, "%d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %d %d %s %d",\
               &evts[i].origin.year,   &evts[i].origin.month, &evts[i].origin.day,  &evts[i].origin.hour, &evts[i].origin.minute, &evts[i].origin.second,\
               &evts[i].dsec,   &evts[i].elat,  &evts[i].dlat, &evts[i].elon, &evts[i].dlon,   &evts[i].depth,\
               &evts[i].ddepth, &evts[i].mag,   &evts[i].nump, &evts[i].nums, evts[i].phase,   &evts[i].id);

        geo2xyz(conf, evts[i].elon, evts[i].elat, evts[i].depth, &evts[i].x, &evts[i].y, &evts[i].z);

        if(evts[i].x < conf.min_x || evts[i].x > conf.max_x ||\
           evts[i].y < conf.min_y || evts[i].y > conf.max_y ||\
           evts[i].z < conf.min_z || evts[i].z > conf.max_z)
        {
            fprintf(stdout,"Event out of model: %d %lf %lf %lf\n", evts[i].id, evts[i].elon, evts[i].elat, evts[i].depth);
            fprintf(stdout,"Event out of model: %d %lf %lf %lf\n", evts[i].id, evts[i].x, evts[i].y, evts[i].z);
            exit(1);
        }

        if(conf.verb == 1)
        {
            fprintf(stdout, "%d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %d %d %s %d\n",\
                    evts[i].origin.year,   evts[i].origin.month, evts[i].origin.day,  evts[i].origin.hour, evts[i].origin.minute, evts[i].origin.second,\
                    evts[i].dsec,   evts[i].elat,  evts[i].dlat, evts[i].elon, evts[i].dlon,   evts[i].depth,\
                    evts[i].ddepth, evts[i].mag,   evts[i].nump, evts[i].nums, evts[i].phase,  evts[i].id);
        }

        evts[i].arrivals = (ARRIV *) calloc(evts[i].nump + evts[i].nums, sizeof(ARRIV));
        for(int j = 0; j < evts[i].nump + evts[i].nums; j++)
        {
            fscanf(fid, "%d %lf %d", &evts[i].arrivals[j].nstn, &evts[i].arrivals[j].tt, &evts[i].arrivals[j].ikps);
            if(conf.verb == 1)
                fprintf(stdout, "%d %lf %d\n", evts[i].arrivals[j].nstn, evts[i].arrivals[j].tt, evts[i].arrivals[j].ikps);
        }

    }
}

void getBathymetry(CONF conf, POINT **bathy)
{
    FILE *fid = NULL;

    if((fid = fopen(conf.file_bath, "r")) == NULL)
    {
        fprintf(stderr, "Can't find file %s\n", conf.file_bath);
        exit(1);
    }

    //FILE *fp = fopen("bathymetry/bath.xyz", "w");


    for(int j = conf.by - 1; j >= 0; j--)
        for(int i = 0; i < conf.bx; i++)
        {
            fscanf(fid, "%lf %lf %lf", &bathy[i][j].x, &bathy[i][j].y, &bathy[i][j].z);
            bathy[i][j].z /= 1000.0;
            //geo2xyz(conf, bathy[i][j].x, bathy[i][j].y, -1.0*bathy[i][j].z, &bathy[i][j].x, &bathy[i][j].y, &bathy[i][j].z);
            //fprintf(fp, "%lf %lf %lf\n", bathy[i][j].x, bathy[i][j].y, bathy[i][j].z);
        }

    //fclose(fp);
    fclose(fid);
}

char* parseLine(FILE *fp, int len)
{
    char *string, *line;

    line = (char*)malloc(200 * sizeof(char));
    string = (char*)malloc(len * sizeof(char));

    if(fgets(line, 200 * sizeof(char), fp) == NULL)
    {
        return(NULL);
    }
    else
    {
        strncpy(string,line,len);
        return(string);
    }
}

// print help information on the screen
void info()
{
    fprintf(stdout, "%s\n", "=========== INFORMATION ===========");
    fprintf(stdout, "%s\n", "OBSLoc verion 1.0");
    fprintf(stdout, "%s\n\n", "========================================");
}
