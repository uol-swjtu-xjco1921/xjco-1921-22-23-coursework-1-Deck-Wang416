#ifndef PGMIMAGE_FILE_H
#define PGMIMAGE_FILE_H

// Define the PGM_Image structure
struct PGM_Image 
{
	unsigned char magic_number[2]; 

	char* commentLine;    

	unsigned int width;   

	unsigned int height; 

	unsigned int maxGray; 

	unsigned char* imageData;  
	     
	long nImageBytes;             
};

// Function to create a new PGM_Image struct and initialize its variables
struct PGM_Image* new_img();

// Function to free memory space occupied by commentLine and imageData
void free_img(struct PGM_Image *img);

#endif