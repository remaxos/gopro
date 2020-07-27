/** \file generator.c
* This file contains the code for the raw image generator. This is a standalone application
* and can be used to futher develop an application that write into a dedicated memory zone
* from the GoPro device
*
* \author Marian-Cristian Rotariu <marian.c.rotariu@gmail.com>
* \version 1.0
* \date 2020
* \bug TODO
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "version.h"
#include "utils.h"
#include "core.h"

uint32_t width = GENERATOR_WIDTH;
uint32_t height = GENERATOR_HEIGHT;
char filename[] = INPUTFILE;

static void usage(void)
{
    fprintf(stderr,
	    "Usage: gopro_generate_raw [ OPTIONS ] \n"
	    "       OPTIONS := { -h[elp] | -v[ersion] | -o[utput] [filename] |\n"
	    "                    -w[idth] [size] | -h[height] [size] }\n");
    exit(-1);
}

static int generate_dummy_file(uint32_t w, uint32_t h, char *out)
{
    unsigned long long i;
    FILE *f = fopen(out, "w");

    if (!f) {
	return -1;
    }	

    fprintf(f, "%u %u\n", w, h);

    for (i = 0; i < w * h; i++) {
	fprintf(f, "%X ", rand() % (1 << 16));
    }
    fprintf(f, "\n");

    fclose(f);
}

int main(int argc, char **argv)
{
    char *output_file = filename;
    int color = 0;

/*
    if (argc <= 1)
	usage();
*/

    /* use current time as seed for random generator */
    srand(time(0));

    while (argc > 1) {
	char *opt = argv[1];

	if (opt[0] != '-')
	    break;

	//printf("%s\n", opt);

	if (matches(opt, "-version") == 0) {
	    printf("generate_dummy utility, version %d\n", VERSION);
	    exit(0);	
	} else if (matches(opt, "-width") == 0) {
	    argc--;
	    argv++;
	    if (argc <= 1)
		usage();
	    width = atoi(argv[1]);
	} else if (matches(opt, "-height") == 0) {
	    argc--;
	    argv++;
	    if (argc <= 1)
		usage();
	    height = atoi(argv[1]);
	} else if (matches(opt, "-output") == 0) {
	    argc--;
	    argv++;
	    if (argc <= 1)
		usage();
	    output_file = argv[1];
	} else if (matches(opt, "-help") == 0) {
	    usage();
	} else {
	    fprintf(stderr,
		    "Option \"%s\" is unknown, try \"generator_dummy -help\".\n",
		    opt);
	    exit(-1);
	}

	argc--;
	argv++;
    }

    generate_dummy_file(width, height, output_file);

    exit(0);
}
