#ifndef PGMIMAGE_FILE_H
#define PGMIMAGE_FILE_H

struct PGM_Image 
{
	unsigned char magic_number[2];
	char *commentLine;
	unsigned int width;
	unsigned int height;
	unsigned int maxGray;
	unsigned char *imageData;
	long nImageBytes;
};

struct PGM_Image* new_img();

void free_img(struct PGM_Image *img);

#endif