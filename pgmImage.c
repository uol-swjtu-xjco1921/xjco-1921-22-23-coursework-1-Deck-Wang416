#include <stdlib.h>
#include <stdio.h>

#include "pgmImage.h"

/* Declare structural variables */
struct PGM_Image* new_img()
{
    struct PGM_Image *img = malloc(sizeof(struct PGM_Image));

    img->magic_number[0] = '0';
    img->magic_number[1] = '0';
    img->commentLine = NULL;
    img->width = 0;
    img->height = 0;
    img->maxGray = 255;
    img->imageData = NULL;
    
    return img;
}

/* Free the memory space occupied by commentLine and imageData */
void free_img(struct PGM_Image *img)
{
    free(img->commentLine);
    free(img->imageData);
}