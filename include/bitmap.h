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

// Ensure the following structure is packed with 1-byte alignment to match the exact layout of the BMP file format.
// This prevents the compiler from adding any padding between the structure members.
#pragma pack(push, 1)

/**
 * Represents the header of a BMP file.
 */
typedef struct
{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

/**
 * Represents the information header of a BMP file.
 */
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

// Restore the previous packing alignment to avoid affecting other parts of the code.
#pragma pack(pop)

/**
 * Saves the image data as a BMP file.
 * The image data is expected to be in RGB format.
 *
 * @param output_path The path of the file to save.
 * @param size The size of the image in pixels.
 * @param image_data A Pointer to the image data in RGB format.
 * @return Status code.
 */
int saveBMP(const char *output_path, ImageSize size, unsigned char *image_data);

/**
 * Sets the pixel at the given position in the image data. Note that the image data is expected to be in RGB format.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param size The size of the image in pixels.
 * @param color The color of the pixel.
 * @param p_image_data A pointer to the image data.
 * @return Status code.
 */
int set_pixel_in_image_data(size_t x, size_t y, ImageSize size, uint32_t color, unsigned char *p_image_data);

#endif  // BITMAP_H