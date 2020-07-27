/** \file core.h
* Description of the file
*
* \author Marian-Cristian Rotariu <marian.c.rotariu@gmail.com>
* \version 1.0
* \date 2020
* \bug TODO
*/

#ifndef _CORE_H_
#define _CORE_H_

#include "utils.h"

#define INPUTFILE 	"input.txt"
#define OUTPUTFILE	"output.txt"

#define GENERATOR_WIDTH  20000
#define GENERATOR_HEIGHT 20000

#define MAX_THREADS 100
#define NTHREADS 1

#define DEPTH 50

/** @struct pixel 
*  @brief This structure represents one of the pixel representations for the solution
*  @var pixel_value
*  Member 'pixel_value' represents the dual-byte representation of the a single pixel
*  @var pixel_position
*  Member 'pixel_position' represents the index inside the raw image representation inside the memory
*/
typedef struct pixel {
    uint16_t pixel_value;
    uint64_t pixel_position;
} pixel;

#endif /* _CORE_H_ */
