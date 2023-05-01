#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"



// prints data stored in header an dheader
void print_meta(BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader)
{
	// prints header info
	printf( "\n"
			"=================================\n"
			"BITMAPFILEHEADER data:\n\n"
			"biType: %c%c\n"
			"biSize: %d\n"
			"biReserved1: %d\n"
			"biReserved2: %d\n"
			"biOffset: %d\n"
			"=================================\n",
			((char *)header)[0], ((char *)header)[1], header->biSize, header->biReserved1, header->biReserved2, header->biOffset);

	// prints dheader info
	printf(	"\n"
			"=================================\n"
			"BITMAPINFOHEADER data: \n\n"
			"biSize: %d\n"
			"biWidth: %d\n"
			"biHeight: %d\n"
			"biPlanes: %d\n"
			"biBitCount: %d\n"
			"biCompression: %d\n"
			"biSizeImage: %d\n"
			"biXPelsPerMeter: %d\n"
			"biYPelsPerMeter: %d\n"
			"biClrUsed: %d\n"
			"biClrImportand: %d\n"
			"==================================\n\n",
			dheader->biSize, dheader->biWidth, dheader->biHeight, dheader->biPlanes, dheader->biBitCount, dheader->biCompression,
			dheader->biSizeImage,dheader->biXPelsPerMeter, dheader->biYPelsPerMeter, dheader->biClrUsed, dheader->biClrImportant);
}




// reads metadata from input file and stores it in vars header and dheader
void get_meta(FILE *bmp_in, BITMAPFILEHEADER *header, BITMAPINFOHEADER *dheader)
{
	// not reading all at once to account for 2 bytes of padding after biType(2 bytes)
	if (fread(header, 2, 1, bmp_in) < 1)
	{
		perror("Error reading header");
		fclose(bmp_in);
		exit(BMP_ERROR);
	}
	
	// check whether first two bytes are 'BM'
	uint16_t biType_hex;
	memcpy(&biType_hex, "BM", sizeof(biType_hex));

	if (header->biType != biType_hex) 
	{
		fclose(bmp_in);
		fprintf(stderr, "Error: file type is not BMP\n");
		exit(BMP_ERROR);
	}
	
	// read rest of the 12 bytes
	if (fread(&(header->biSize), BITMAPFILEHEADER_SIZE - 2, 1, bmp_in) < 1) 
	{
		perror("Error reading header");
		fclose(bmp_in);
		exit(BMP_ERROR);
	}

	// read dib header info (no padding to worry about)
	if (fread(dheader, BITMAPINFOHEADER_SIZE, 1, bmp_in) < 1)
	{
		perror("Error reading dheader");
		fclose(bmp_in);
		exit(BMP_ERROR);
	}
	
	// check whether dheader size is 40
	if (dheader->biSize != 40)
	{
		fclose(bmp_in);
		fprintf(stderr, "Error: biSize must be 40\n");
		exit(BMP_ERROR);
	}

	// check whether commpression method is BI_RGB (biCompression = 0 / no compression)
	if (dheader->biPlanes != 1)
	{
		fclose(bmp_in);
		fprintf(stderr, "Error: biPlanes must be 1\n");
		exit(BMP_ERROR);
	}

	// check whether image is 1/2/4/8/16 bpp
	if (dheader->biBitCount != 1 && dheader->biBitCount != 2 && dheader->biBitCount != 4 && 
		dheader->biBitCount != 8 && dheader->biBitCount != 16)
	{
		fclose(bmp_in);
		fprintf(stderr, "Error: only 1/2/4/8/16 bpp is supported\n");
		exit(BMP_ERROR);
	}

	// check whether commpression method is BI_RGB (biCompression = 0 / no compression)
	if (dheader->biCompression != 0)
	{
		fclose(bmp_in);
		fprintf(stderr, "Error: only BI_RGB compression method is supported\n");
		exit(BMP_ERROR);
	}
}
