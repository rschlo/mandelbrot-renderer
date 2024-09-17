#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

#define ERROR_FILE_OPEN -1

/**
 * Represents the size of an image. 
 */
typedef struct
{
    size_t width;
    size_t height;
} ImageSize;

#pragma pack(push, 1)

typedef struct
{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

/**
 * Saves the image data as a BMP file. 
 * The image data is expected to be in RGB format. 
 */
int saveBMP(const char *filename, ImageSize size, unsigned char *image_data);


/**
 * Sets the pixel at the given position in the image data. Note that the image data is expected to be in RGB format. 
 */
int set_pixel_in_image_data(size_t x, size_t y, ImageSize size, uint32_t color, unsigned char *p_image_data);

#endif // BITMAP_H