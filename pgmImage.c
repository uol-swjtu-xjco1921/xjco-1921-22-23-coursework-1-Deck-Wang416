#include <stdlib.h>
#include <stdio.h>

#include "pgmEcho_func.h"

/* Create new PGM_Image structs and initialize variables */
struct PGM_Image* new_img()
{
    struct PGM_Image *img = malloc(sizeof(struct PGM_Image));

    /* Initialize magic_number */
    img->magic_number[0] = '0';

    img->magic_number[1] = '0';

    /* Initialize commentLine as NULL */
    img->commentLine = NULL;

    /* Initialize width, height and maxGray with default values */
    img->width = 0;

    img->height = 0;

    img->maxGray = 255;

    /* Initialize imageData as NULL */
    img->imageData = NULL;
    
    return img;
}

/* Free the memory space occupied by commentLine and imageData fields within PGM_Image struct */
void free_img(struct PGM_Image *img)
{
    /* Free the memory allocated for commentLine*/
    if (img->commentLine != NULL) 
    {
        free(img->commentLine);
    }

    /* Free the memory allocated for imageData */
    if (img->imageData != NULL) 
    {
        free(img->imageData);
    }
}