/** \file core.c
* This file contains the core part of the application 
*
* \author Marian-Cristian Rotariu <marian.c.rotariu@gmail.com>
* \version 1.0
* \date 2020
* \bug TODO
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

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
    uint32_t w, h;
    uint16_t *pixel_map;
    uint16_t pixel_value;

    FILE *f = fopen(input_file, "r");
    if (!f) {
	printf("Cannot open input file!\n");
	return -ENOENT;
    }

    fscanf(f, "%u %u", &w, &h);

    mem = (uint8_t *) malloc(sizeof(uint16_t) * w * h + 2 * sizeof(unsigned long));
    if (!mem) {
	printf("Cannot allocate memory!\n");
	return -ENOMEM;
    }

    memcpy(mem, &w, sizeof(w));
    memcpy(mem + sizeof(w), &h, sizeof(h));

    pixel_map = (uint16_t *)(mem + sizeof(w) + sizeof(h));

    for (i = 0; i < w * h; i++) {
	int n = fscanf(f, "%hx ", &pixel_value);	
	memcpy(pixel_map + i, &pixel_value, sizeof(pixel_value));
    }

    fclose(f);
}

/**
* TODO
!*
* @param values Container whose values are summed.
* @return sum of `values`, or 0.0 if `values` is empty.
*/
static void *thread_work(void *arg)
{
    struct thread_info *tinfo = arg;
    solution *s = tinfo->solution;
    uint16_t *pixels = tinfo->pixels;
    unsigned long long i;

    printf("Starting worker with ID %d: %d %d\n", tinfo->thread_num, tinfo->start, tinfo->stop);

    for (i = tinfo->start; i <= tinfo->stop; i++) {

	pixel *p = (pixel *)malloc(sizeof(pixel));
	if (!p) {
	    printf("Cannot allocate space in thread %d!\n", tinfo->thread_num);
	    /* TODO: what to do inside a thread */
	}

	p->pixel_value = *(pixels + i);
	p->pixel_position = i;

	/* first 50 elements should be added nevertheless */
	if (s->count <= DEPTH || tinfo->func(s->pixels->pixel, p) <= 0) {
	    s->pixels = list_add_in_order(s->pixels, p);
	    s->count++;
	}

	if (s->count > DEPTH) {
	    s->pixels = list_remove_first(s->pixels);
	    s->count--;
	}
    }
}

/**
* TODO
!*
* @param values Container whose values are summed.
* @return sum of `values`, or 0.0 if `values` is empty.
*/
static int find_overexposed_pixels_parallel(uint8_t *mem, compare_pixels func, int num_threads, solution *s)
{
     uint32_t *pw, *ph; 
     uint32_t w, h;
     uint64_t i;
     uint16_t *pixels;
     int err;
     solution *ls; /* local solution */

     pw = (uint32_t *)mem;
     w = *pw;
     ph = (uint32_t *)(mem + sizeof(w));
     h = *ph;

     s->width = w;
     s->height = h;
     s->count = DEPTH;

     pixels = (uint16_t *)(mem + sizeof(w) + sizeof(h));

     /* allocate */
     thread_info *p[MAX_THREADS];

     unsigned long long chunk_size = w * h / num_threads;

     printf("%s() chunk_size:%llu\n", __func__, chunk_size);

     for (i = 0; i < num_threads; i++) {
	 p[i] = (thread_info *)malloc(sizeof(thread_info));
	 if (!p[i]) {
	     printf("Cannot allocate memory!\n");
	     return -ENOMEM; 
	 }

	 p[i]->start = i * chunk_size;
	 if (i == num_threads - 1)
	     p[i]->stop = w * h;
	 else
	     p[i]->stop =  (i + 1) * chunk_size;

	 p[i]->pixels = pixels;
	 p[i]->func = func;

	 ls = (solution *)malloc(sizeof(solution));
	 if (!ls) {
	     printf("Cannot allocate memory!\n");
	     return -ENOMEM;
	 }

	 ls->width = w;
	 ls->height = h;
	 ls->count = 0;
	 ls->pixels = NULL;

	 p[i]->solution = ls;
     }

     for (i = 0; i < num_threads; i++) {
	 err = pthread_create(&p[i]->thread_id, NULL, &thread_work, p[i]);
	 if (err)
	 {
	     printf("Thread creation failed!\n");
	     return err;
	 }

	 p[i]->thread_num = i;
     }

     for (i = 0; i < num_threads; i++) {
	 err = pthread_join(p[i]->thread_id, NULL);
	 if (err)
	 {
	     printf("Failed to join threads!\n");
	     return err;
	 }
     }

     for (i = 0; i < num_threads; i++) {
	  err = merge_solutions(s, p[i]->solution);
	  if (err) {
	      printf("Cannot merge solutions\n");
	      return err;
	  }
     }

     return 0;
}

/**
* TODO
*
* @param values Container whose values are summed.
* @return sum of `values`, or 0.0 if `values` is empty.
*/
static int find_overexposed_pixels_sequential(uint8_t *mem, compare_pixels func, solution *s)
{
     uint32_t *pw, *ph; 
     uint32_t w, h;
     uint64_t i;
     uint16_t *pixels;

     pw = (uint32_t *)mem;
     w = *pw;
     ph = (uint32_t *)(mem + sizeof(w));
     h = *ph;

     s->width = w;
     s->height = h;

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
	 if (s->count <= DEPTH || func(s->pixels->pixel, p) <= 0) {
	     s->pixels = list_add_in_order(s->pixels, p);
	     s->count++;
	 }

	 if (s->count > DEPTH) {
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
	 printf("Not enough memory!\n");
	 return -ENOMEM;
    }
    s->width = 0;
    s->height = 0;
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
    if (ret) {
	return ret;
    }

    /* dump_mempic(mem); */

    /* TODO: use OpenCV to load standard images */
    /* cvLoadImageM(const char* filename, int iscolor=CV_LOAD_IMAGE_COLOR); */

#if 0
    ret = find_overexposed_pixels_sequential(mem, compare, s);
    if (ret) {
	return ret;
    }
#endif

    ret = find_overexposed_pixels_parallel(mem, compare, 3, s);
    if (ret) {
	return ret;
    }

    ret = print_solution(s, output_file);
    if (ret) {
	return ret;
    }

    return 0;
}
