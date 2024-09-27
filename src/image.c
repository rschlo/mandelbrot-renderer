#include "../include/image.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/codes.h"
#include "../include/colors.h"

int set_pixel_in_image_data(size_t x, size_t y, ImageSize size, uint32_t color, unsigned char *p_image_data) {
    if (y > SIZE_MAX / size.width || y * size.width > SIZE_MAX - x || x + y * size.width > SIZE_MAX / 3) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }

    int index = (x + y * size.width) * 3;

    p_image_data[index] = get_blue(color);
    p_image_data[index + 1] = get_green(color);
    p_image_data[index + 2] = get_red(color);
    return SUCCESS;
}

/**
 * Saves the image data as a BMP file.
 * The image data is expected to be in RGB format.
 *
 * @param output_path The path of the file to save.
 * @param size The size of the image in pixels.
 * @param image_data A pointer to the image data in RGB format.
 * @return Status code.
 */
int _save_bmp(const char *output_path, ImageSize size, unsigned char *image_data) {
    BitmapFileHeader file_header;
    BitmapInfoHeader info_header;

    if (size.width > SIZE_MAX / 3 || 3 * size.width > SIZE_MAX / size.height) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }

    int image_size = size.width * size.height * 3;  // 3 bytes per pixel (RGB)
    file_header.type = 0x4D42;                      // "BM" in hex

    if (image_size > SIZE_MAX - sizeof(BitmapFileHeader) - sizeof(BitmapInfoHeader)) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }

    file_header.size = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + image_size;
    file_header.reserved1 = 0;
    file_header.reserved2 = 0;
    file_header.offset_bits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    info_header.header_size = sizeof(BitmapInfoHeader);
    info_header.width = size.width;
    info_header.height = size.height;
    info_header.num_planes = 1;
    info_header.bits_per_pixel = 24;  // 24 bit (RGB)
    info_header.compression = 0;      // No compression
    info_header.image_size = image_size;
    info_header.pixels_per_meter_x = 0;
    info_header.pixels_per_meter_y = 0;
    info_header.num_colors = 0;
    info_header.num_important_colors = 0;

    FILE *file = fopen(output_path, "wb");
    if (!file) {
        return ERROR_FILE_ACCESS;
    }

    fwrite(&file_header, sizeof(BitmapFileHeader), 1, file);
    fwrite(&info_header, sizeof(BitmapInfoHeader), 1, file);

    // This works without overflow because of the checks above (size.width * size.height * 3 <= SIZE_MAX)
    for (int y = size.height - 1; y >= 0; y--) {
        fwrite(image_data + (y * size.width * 3), 3, size.width, file);
    }

    fclose(file);
    return SUCCESS;
}

int malloc_image_data(ImageSize size, unsigned char **p_p_image_data) {
    if (size.width == 0 || size.height == 0) {
        return ERROR_IMAGE_SIZE_0;
    }
    if (size.width > SIZE_MAX / 3 || 3 * size.width > SIZE_MAX / size.height) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }
    size_t malloc_size = size.width * size.height * 3;

    unsigned char *p_memory = (unsigned char *)malloc(malloc_size);
    if (p_memory == NULL) {
        return ERROR_MEMORY;
    }
    *p_p_image_data = p_memory;
    return SUCCESS;
}

int export_and_free(unsigned char *p_image_data, ImageSize size, const char *output_path) {
    int status_export = _save_bmp(output_path, size, p_image_data);
    if (status_export < 0) {
        return status_export;
    }
    free(p_image_data);
    return SUCCESS;
}
