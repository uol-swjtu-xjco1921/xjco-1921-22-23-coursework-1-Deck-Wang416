#include <stdlib.h>
#include <stdio.h>

#include "pgmEcho_func.h"

// Create new PGM_Image structs and initialize variables
struct PGM_Image* new_img()
{
    struct PGM_Image *img = malloc(sizeof(struct PGM_Image));

    // Initialize members
    img->magic_number[0] = '0';

    img->magic_number[1] = '0';

    img->commentLine = NULL;

    img->width = 0;

    img->height = 0;

    img->maxGray = 255;

    img->imageData = NULL;
    
    return img;
}

// Free memory
void free_img(struct PGM_Image *img)
{
    // Free the memory allocated for commentLine
    if (img->commentLine != NULL) 
    {
        free(img->commentLine);
    }

    // Free the memory allocated for imageData
    if (img->imageData != NULL) 
    {
        free(img->imageData);
    }
}