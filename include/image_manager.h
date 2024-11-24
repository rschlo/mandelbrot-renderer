#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <stdint.h>

#include "config.h"

typedef struct {
    size_t width;
    size_t height;
} ImageSize;

typedef struct {
    ImageSize size;
    unsigned char* data;
} ImageData;

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
 * Sets the pixel at the given position in the image data.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel. From LSB to MSB: blue (8 bit), green (8 bit), red (8 bit). Alpha value will be ignored.
 * @param p_image_data A pointer to the image data.
 * @return Status code.
 */
int set_pixel_in_image_data(size_t x, size_t y, uint32_t color, ImageData* p_image_data);

/**
 * Saves the image data to a file and frees the memory.
 *
 * @param p_image_data The image data to save.
 * @param output_path The path to save the image to.
 * @return Status code.
 */
int export_and_free(ImageData* p_image_data, const char* output_path);

/**
 * Calculates the size of the image and then allocates memory for the image data.
 * The image size is calculated based on the viewport and the width of the image so that the aspect ratio is preserved.
 * The memory must be freed by the caller.
 *
 * @param viewport The viewport of the image.
 * @param width The width of the image in pixels.
 * @param p_p_image_data A pointer to the pointer to where the image data should be stored.
 * @return Status code.
 */
int create_image_data(Viewport viewport, size_t width, ImageData** p_p_image_data);

#endif  // IMAGE_MANAGER_H