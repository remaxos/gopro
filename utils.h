/** \file utils.h
* Description of the file
*
* \author Marian-Cristian Rotariu <marian.c.rotariu@gmail.com>
* \version 1.0
* \date 2020
* \bug TODO
*/

#include <stdint.h>

#ifndef _UTILS_H_
#define _UTILS_H_

/** @struct pixel 
*  @brief This structure blah blah blah...
*  @var foreignstruct::a 
*  Member 'a' contains...
*  @var foreignstruct::b 
*  Member 'b' contains...
*/
typedef struct pixel {
    uint16_t pixel_value;
    uint64_t pixel_position;
} pixel;

/** @struct list 
*  @brief This structure blah blah blah...
*  @var foreignstruct::a 
*  Member 'a' contains...
*  @var foreignstruct::b 
*  Member 'b' contains...
*/
typedef struct list {
    pixel *pixel;
    struct list *next;
} list;

/** @struct solution_struct 
*  @brief This structure blah blah blah...
*  @var foreignstruct::a 
*  Member 'a' contains...
*  @var foreignstruct::b 
*  Member 'b' contains...
*/
typedef struct solution_struct {
    uint32_t width;
    uint32_t height;
    int count;    
    list *pixels; 
} solution;

typedef int compare_pixels(pixel *p1, pixel *p2);

int matches(const char *cmd, const char *pattern);
void dump_mempic(uint8_t *mem);
int compare(pixel *p1, pixel *p2);
list *list_add_in_order(list *l, pixel *p);
list *list_remove_first(list *l);
void list_print(list *l, uint32_t w, FILE *f);
int print_solution(solution *s, char *file);

#endif /* _UTILS_H_ */
