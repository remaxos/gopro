#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "version.h"
#include "utils.h"

unsigned long width = 10;
unsigned long height = 20;
char filename[] = "input.txt";

static void usage(void)
{
    fprintf(stderr,
	    "Usage: generate_dummy [ OPTIONS ] \n"
	    "       OPTIONS := { -h[elp] | -v[ersion] | -o[utput] [filename] |\n"
	    "                    -w[idth] [size] | -h[height] [size] }\n");
    exit(-1);
}

static int generate_dummy_file(unsigned long w, unsigned long h, char *out)
{
    int i;
    FILE *f = fopen(out, "w");

    if (!f) {
	return -1;
    }	

    fprintf(f, "%lu %lu\n", w, h);

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

	printf("%s\n", opt);

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
