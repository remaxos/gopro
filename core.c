#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "version.h"
#include "utils.h"
#include "core.h"

uint8_t *mem;

/* add debug */
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

    FILE *f = fopen(input_file, "r");
    if (!f) {
	/* ... */
    }

    fscanf(f, "%lu %lu", &w, &h);

    printf("w=%lu h=%lu\n", w, h);
    fflush(stdout);


    mem = (uint8_t *) malloc(sizeof(uint16_t) * w * h + 2 * sizeof(unsigned long));
    if (!mem) {
	/* ... */
    }

    memcpy(mem, &w, sizeof(w));
    memcpy(mem + sizeof(w), &h, sizeof(h));

    pixel_map = (uint16_t *)(mem + sizeof(w) + sizeof(h));

    printf("w=%lu h=%lu\n", w, h);
    for (i = 0; i < w * h; i++) {
	int n = fscanf(f, "%hx ", &pixel_value);	
	//printf("n=%d\n", n);
	printf("i=%lld val=%x\n", i, pixel_value);
	memcpy(pixel_map + i, &pixel_value, sizeof(pixel_value));
    }

    fclose(f);
}


/**
* TODO
*
* @param values Container whose values are summed.
* @return sum of `values`, or 0.0 if `values` is empty.
*/
static int find_overexposed_pixels(uint8_t *mem, compare_pixels func, solution *s)
{
     unsigned long *pw, *ph; 
     unsigned long w, h;
     uint64_t i;
     uint16_t *pixels;

     pw = (unsigned long*)mem;
     w = *pw;
     ph = (unsigned long *)(mem + sizeof(w));
     h = *ph;

     

     pixels = (uint16_t *)(mem + sizeof(w) + sizeof(h));

     for (i = 0; i < w * h; i++) {
	 pixel *p = (pixel *)malloc(sizeof(pixel));
	 if (!p) {
	     /* the search cannot go on */
	     /* TODO */
	 }

	 p->pixel_value = *(pixels + i);
	 p->pixel_position = i;

	 /* first 50 elements should be added nevertheless */
	 if (s->count <= 50 || func(s->pixels->pixel, p) <= 0) {
	     s->pixels = list_add_in_order(s->pixels, p);
	     s->count++;
	 }

	 if (s->count > 50) {
	     s->pixels = list_remove_first(s->pixels);
	     s->count--;
	 }
     }

     return 0;
}


int main(int argc, char **argv)
{
    solution *s;
    int color = 0;
    int ret;

    char default_input_file[] = INPUTFILE;
    char default_output_file[] = OUTPUTFILE;
    char *input_file = default_input_file;
    char *output_file = default_output_file;

    s = (solution *)malloc(sizeof(solution));
    if (!s) {
	 /* TODO */
    }
    s->count = 0; 
    s->pixels = NULL;

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


    ret = load_image(input_file);
    dump_mempic(mem);
    //cvLoadImageM(const char* filename, int iscolor=CV_LOAD_IMAGE_COLOR);

    ret = find_overexposed_pixels(mem, compare, s);
    if (ret) {
	/* TODO */
    }

    print_solution(s);

    return 0;
}
