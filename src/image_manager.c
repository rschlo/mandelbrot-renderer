#include "../include/image_manager.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/color_utilities.h"
#include "../include/status_manager.h"

/**
 * Saves the image data as a BMP file.
 * The image data is expected to be in RGB format.
 *
 * @param output_path The path of the file to save.
 * @param size The size of the image in pixels.
 * @param p_pixel_data A pointer to the image data in RGB format.
 * @return Status code.
 */
int _save_bmp(const char *output_path, ImageSize size, unsigned char *p_pixel_data) {
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
        fwrite(p_pixel_data + (y * size.width * 3), 3, size.width, file);
    }

    fclose(file);
    return SUCCESS;
}

/**
 * Maps the viewport size to the image size. The aspect ratio is kept.
 * Calculates the image size based on the width and the viewport.
 * It keeps the aspect ratio and calculates the height.
 *
 * @param viewport The viewport of the complex plane
 * @param image_width The width of the image in pixels
 * @param p_image_size The pointer to store the calculated image size
 * @return Status code
 */
int _calc_image_size(Viewport viewport, size_t image_width, ImageSize *p_image_size) {
    if (viewport.upper_right.real == viewport.lower_left.real ||
        viewport.upper_right.imag == viewport.lower_left.imag) {
        return ERROR_INVALID_VIEWPORT;
    }

    p_image_size->width = image_width;
    double viewport_width = fabs(viewport.upper_right.real - viewport.lower_left.real);
    double viewport_height = fabs(viewport.upper_right.imag - viewport.lower_left.imag);

    if (viewport_width < 1 && viewport_height > SIZE_MAX * viewport_width) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }
    double aspect_ratio = viewport_height / viewport_width;

    if (p_image_size->width > SIZE_MAX / aspect_ratio) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }
    p_image_size->height = (size_t)(image_width * aspect_ratio);

    if (p_image_size->width == 0 || p_image_size->height == 0) {
        return ERROR_IMAGE_SIZE_0;
    }

    return SUCCESS;
}

/**
 * Mallocs memory for the image data.
 *
 * @param size The size of the image in pixels.
 * @param p_p_image_data A pointer to the pointer where the image data should be stored.
 * @return Status code.
 */
int _malloc_image_data(ImageSize size, ImageData **p_p_image_data) {
    if (size.width == 0 || size.height == 0) {
        return ERROR_IMAGE_SIZE_0;
    }
    if (size.width > SIZE_MAX / 3 || 3 * size.width > SIZE_MAX / size.height) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }
    size_t malloc_size = size.width * size.height * 3;

    unsigned char *p_memory = (unsigned char *)malloc(malloc_size);
    if (p_memory == NULL) {
        free(p_memory);
        return ERROR_MEMORY_ALLOC;
    }
    ImageData *p_image_data = (ImageData *)malloc(sizeof(ImageData));
    if (p_image_data == NULL) {
        free(p_memory);
        return ERROR_MEMORY_ALLOC;
    }

    p_image_data->size = size;
    p_image_data->data = p_memory;
    *p_p_image_data = p_image_data;
    return SUCCESS;
}

int create_image_data(Viewport viewport, size_t image_width, ImageData **p_p_image_data) {
    ImageSize size;
    int status = _calc_image_size(viewport, image_width, &size);
    if (status < 0) {
        return status;
    }
    status = _malloc_image_data(size, p_p_image_data);
    if (status < 0) {
        return status;
    }
    return SUCCESS;
}

int export_and_free(ImageData *p_image_data, const char *output_path) {
    ImageSize size = p_image_data->size;
    int status_export = _save_bmp(output_path, size, p_image_data->data);
    if (status_export < 0) {
        return status_export;
    }
    free(p_image_data);
    return SUCCESS;
}

int set_pixel_in_image_data(size_t x, size_t y, uint32_t color, ImageData *p_image_data) {
    ImageSize size = p_image_data->size;
    if (y > SIZE_MAX / size.width || y * size.width > SIZE_MAX - x || x + y * size.width > SIZE_MAX / 3) {
        return ERROR_ARITHMETIC_OVERFLOW;
    }

    int index = (x + y * size.width) * 3;

    p_image_data->data[index] = get_blue(color);
    p_image_data->data[index + 1] = get_green(color);
    p_image_data->data[index + 2] = get_red(color);
    return SUCCESS;
}
