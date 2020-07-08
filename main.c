#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "version.h"
#include "utils.h"

char filename[] = "input.txt";
char *mem;

static void usage(void)
{
    fprintf(stderr,
	    "Usage: generate_dummy [ OPTIONS ] \n"
	    "       OPTIONS := { -h[elp] | -v[ersion] | -i[input] [filename] \n");
    exit(-1);
}

static int load_image(char *input_file)
{
    unsigned long long i;
    unsigned long w, h;
    uint16_t *pixel_map;
    uint16_t pixel_value;

    printf("test\n");
    fflush(stdout);

    FILE *f = fopen(input_file, "r");
    if (!f) {
	/* ... */
    }

    fscanf(f, "%lu %lu", &w, &h);

    printf("w=%lu h=%lu\n", w, h);
    fflush(stdout);


    mem = (char *) malloc(sizeof(uint16_t) * w * h + 2 * sizeof(unsigned long));
    if (!mem) {
	/* ... */
    }

    memcpy(mem, &w, sizeof(w));
    memcpy(mem + sizeof(unsigned long), &h, sizeof(h));

    pixel_map = mem + 2 * sizeof(unsigned long);

    for (i = 0; i < w * h; i++) {
	fscanf(f, "%X", &pixel_value);	
	memcpy(pixel_map + i, &pixel_value, sizeof(uint16_t));
    }

    fclose(f);
}



int main(int argc, char **argv)
{
    char *input_file = filename;
    int color = 0;
    int ret;

/*
    if (argc <= 1)
	usage();
*/

    while (argc > 1) {
	char *opt = argv[1];

	if (opt[0] != '-')
	    break;

	printf("%s\n", opt);

	if (matches(opt, "-version") == 0) {
	    printf("find_exposure utility, version %d\n", VERSION);
	    exit(0);	
	} else if (matches(opt, "-input") == 0) {
	    argc--;
	    argv++;
	    if (argc <= 1)
		usage();
	    input_file = argv[1];
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


    ret = load_image(input_file);
    dump_mempic(mem);
    //cvLoadImageM(const char* filename, int iscolor=CV_LOAD_IMAGE_COLOR);

    return 0;
}
