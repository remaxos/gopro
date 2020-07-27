/** \file utils.c
* Various utilites other than the algorithm for the source code
*
* \author Marian-Cristian Rotariu <marian.c.rotariu@gmail.com>
* \version 1.0
* \date 2020
* \bug TODO
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include "utils.h"
#include "core.h"

void dump_mempic(uint8_t *mem)
{
    unsigned long *pw, *ph; 
    unsigned long w, h;
    uint16_t i;
    uint16_t *pixels;

    pw = (unsigned long*)mem;
    w = *pw;
    ph = (unsigned long *)(mem + sizeof(w));
    h = *ph;

    pixels = (uint16_t *)(mem + sizeof(w) + sizeof(h));

    printf("width=%lu height=%lu\n", w, h);
    for (i = 0; i < w * h; i++) {
	printf("%04X ", *(pixels + i));
    }
    printf("\n");
}

/**
* TODO
*
* @param values Container whose values are summed.
* @return sum of `values`, or 0.0 if `values` is empty.
*/
int compare(pixel *p1, pixel *p2)
{
    return p1->pixel_value - p2->pixel_value;
}

/**
* TODO
*
* @param values Container whose values are summed.
* @return sum of `values`, or 0.0 if `values` is empty.
*/
int matches(const char *cmd, const char *pattern)
{
	int len = strlen(cmd);

	if (len > strlen(pattern))
		return -1;
	return memcmp(pattern, cmd, len);
}

int list_size(list *l)
{
    int count;
    list *iter;

    count = 0;
    iter = l;
    

    while (iter) {
	count++;
	iter = iter->next;
    }

    return count;
}

list *list_add_to_end(list *l, pixel *p)
{
    list *n;
    list *idx;

    n = (list *)malloc(sizeof(list));
    n->pixel = p;
    n->next = NULL;

    if (l == NULL) {
	return n; 
    }

    idx = l;
    while (idx->next != NULL) {
	idx = idx->next;
    }

    idx->next = n;

    return l;
}

/**
* TODO
*
* @param values Container whose values are summed.
* @return sum of `values`, or 0.0 if `values` is empty.
*/
list *list_add_in_order(list *l, pixel *p) {
    list *n;
    list *idx;

    n = (list *)malloc(sizeof(list));
    if (!n) {
	/* TODO */
    }

    n->pixel = p;
    n->next = NULL;

    if (l == NULL) {
	return n; 
    }

    if (compare(l->pixel, p) > 0) {
	n->next = l;
	return n;
    }
    
    idx = l;
    while (idx->next != NULL) {
	if (compare(idx->next->pixel, p) >= 0) {
	    n->next = idx->next;
	    idx->next = n;
	    return l;
	}

	idx = idx->next;
    }

    idx->next = n;

    return l;
}

#if 0
/* unuseful function */
list *list_search(list *l, pixel *p) {
    list *idx;

    idx = l;
    while (idx) {
	if (idx->pixel == pixel)
	    return idx;

	idx = idx->next;
    }

    return NULL;
}
#endif

list *list_del(list *l, list *n) {
    list *idx;

    if (l == n) {
	list *ret = l->next;
	free(l);

	return ret;
    }

    idx = l;
    while (idx->next) {
	if (idx->next == n) {
	    idx->next = idx->next->next;
	    free(n);
	    return l;
	}

	idx = idx->next;
    }

    return l;
}

void list_print(list *l, uint32_t width, FILE *file) {
    list *idx;
    idx = l;

    fprintf(file, "\n");
    while(idx) {
	fprintf(file, "x=%ld y=%ld val=%d\n", idx->pixel->pixel_position / width,
		idx->pixel->pixel_position % width, idx->pixel->pixel_value); 
	idx = idx->next;
    }
}

list *list_remove_first(list *l)
{
    list *new_list = l->next;

    /* TODO: generic free approach */ 
    free(l->pixel);
    free(l);

    return new_list;
}

#if 0
void list_fprint(list *l, FILE *f) {
    list *idx;

    idx = l;

    fprintf(f, "\n[");
    while(idx) {
	fprintf(f, "(%d,%d)", idx->r->a, idx->r->b); 
	idx = idx->next;
    }
    fprintf(f, "]\n");
    fflush(f);
}
#endif

/* merge two solutions s1 & s2 into s*/
int merge_solutions(solution *acc, solution *s)
{
    list *l;
    int i;
    int extra_size;

    /* the two solutions should be compatible */
    if (acc->count != s->count || acc->width != s->width || acc->height != s->height) {
	/* TODO: find a proper return code */
	return -1;
    }

    l = s->pixels;
    while (l) {
	acc->pixels = list_add_in_order(acc->pixels, l->pixel);
	l = l->next;
    }

    if (list_size(acc->pixels) < acc->count) {
	/* TODO: find a proper return code */
	return -1;
    }

    /* the extra 'smaller' elements should be removed */
    extra_size = list_size(acc->pixels) - acc->count;
    for (i = 0; i < extra_size; i++) {
	if (acc->pixels == NULL) {
	    printf("Cannot remove extra elements from list!\n");
	    return -EFAULT;
	}

	acc->pixels = acc->pixels->next;
    }

    return 0;
}

int print_solution(solution *s, char *file)
{
    FILE *f = fopen(file, "w");
    if (!f) {
	/* TODO */
	return -1;
    }

    fprintf(f, "Raw image size: %dx%d\n", s->width, s->height);
    fprintf(f, "Number of overexposed pixels: %d\n", s->count);
    list_print(s->pixels, s->width, f);

    fclose(f); 
}
