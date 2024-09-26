#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

#define ERROR_FILE_OPEN -1
#define WARNING_IMAGE_SIZE_0 2

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
 * @struct BitmapFileHeader
 * @brief Represents the header of a BMP file.
 *
 * For more information, see https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header.
 */
typedef struct
{
    unsigned short type;       // The header field, must be 0x4D42 (hex for "BM")
    unsigned int size;         // The size of the BMP file in bytes
    unsigned short reserved1;  // Reserved; actual value depends on the application that creates the image, if created manually can be 0
    unsigned short reserved2;  // Reserved; actual value depends on the application that creates the image, if created manually can be 0
    unsigned int offset_bits;  // The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.
} BitmapFileHeader;

/**
 * Represents the information header of a BMP file.
 * For more information, see https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header.
 */
typedef struct
{
    unsigned int header_size;           // The size of this header, in bytes (40)
    int width;                          // The bitmap width in pixels (signed integer)
    int height;                         // The bitmap height in pixels (signed integer)
    unsigned short num_planes;          // The number of color planes (must be 1)
    unsigned short bits_per_pixel;      // The number of bits per pixel, which is the color depth of the image.
    unsigned int compression;           // The compression method being used
    unsigned int image_size;            // The image size
    int pixels_per_meter_x;             // The horizontal resolution of the image
    int pixels_per_meter_y;             // The vertical resolution of the image
    unsigned int num_colors;            // The number of colors in the color palette, or 0 to default to 2^n
    unsigned int num_important_colors;  // The number of important colors used, or 0 when every color is important; generally ignored
} BitmapInfoHeader;

// Restore the previous packing alignment to avoid affecting other parts of the code.
#pragma pack(pop)

/**
 * Saves the image data as a BMP file.
 * The image data is expected to be in RGB format.
 *
 * @param output_path The path of the file to save.
 * @param size The size of the image in pixels.
 * @param image_data A pointer to the image data in RGB format.
 * @return Status code.
 */
int saveBMP(const char *output_path, ImageSize size, unsigned char *image_data);

/**
 * Sets the pixel at the given position in the image data. Note that the image data is expected to be in RGB format and (3*width*height) long
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param size The size of the image in pixels.
 * @param color The color of the pixel.
 * @param p_image_data A pointer to the image data.
 * @return Status code.
 */
int set_pixel_in_image_data(size_t x, size_t y, ImageSize size, uint32_t color, unsigned char *p_image_data);

/**
 * Mallocs memory for the image data.
 *
 * @param size The size of the image in pixels.
 * @param p_p_image_data A pointer to the pointer where the image data should be stored.
 * @return Status code.
 */
int malloc_image_data(ImageSize size, unsigned char **p_p_image_data);

#endif  // BITMAP_H