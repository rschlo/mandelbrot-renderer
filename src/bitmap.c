#include "..\include\bitmap.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/colors.h"
#include "..\include\codes.h"

int set_pixel_in_image_data(size_t x, size_t y, ImageSize size, uint32_t color, unsigned char *p_image_data) {
    if (y > SIZE_MAX / size.width || y * size.width > SIZE_MAX - x || x + y * size.width > SIZE_MAX / 3) {
        return ERROR_OVERFLOW;
    }

    int index = (x + y * size.width) * 3;

    p_image_data[index] = get_blue(color);
    p_image_data[index + 1] = get_green(color);
    p_image_data[index + 2] = get_red(color);
    return SUCCESS;
}

int saveBMP(const char *output_path, ImageSize size, unsigned char *image_data) {
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;

    if (size.width > SIZE_MAX / 3 || 3 * size.width > SIZE_MAX / size.height) {
        return ERROR_OVERFLOW;
    }

    int image_size = size.width * size.height * 3;  // 3 Bytes pro Pixel (RGB)

    file_header.bfType = 0x4D42;  // "BM" in hex

    if (image_size > SIZE_MAX - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER)) {
        return ERROR_OVERFLOW;
    }

    file_header.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + image_size;
    file_header.bfReserved1 = 0;
    file_header.bfReserved2 = 0;
    file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    info_header.biSize = sizeof(BITMAPINFOHEADER);
    info_header.biWidth = size.width;
    info_header.biHeight = size.height;
    info_header.biPlanes = 1;
    info_header.biBitCount = 24;    // 24 Bit (RGB)
    info_header.biCompression = 0;  // Keine Kompression
    info_header.biSizeImage = image_size;
    info_header.biXPelsPerMeter = 0;
    info_header.biYPelsPerMeter = 0;
    info_header.biClrUsed = 0;
    info_header.biClrImportant = 0;

    FILE *file = fopen(output_path, "wb");
    if (!file) {
        return ERROR_FILE_OPEN;
    }

    fwrite(&file_header, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&info_header, sizeof(BITMAPINFOHEADER), 1, file);

    // This works without overflow because of the checks above (size.width * size.height * 3 <= SIZE_MAX)
    for (int y = size.height - 1; y >= 0; y--) {
        fwrite(image_data + (y * size.width * 3), 3, size.width, file);
    }

    fclose(file);
    return SUCCESS;
}

int malloc_image_data(ImageSize size, unsigned char **p_p_image_data) {
    if (size.width == 0 || size.height == 0) {
        return WARNING_IMAGE_SIZE_0;
    }
    if (size.width > SIZE_MAX / 3 || 3 * size.width > SIZE_MAX / size.height) {
        return ERROR_OVERFLOW;
    }
    size_t malloc_size = size.width * size.height * 3;

    unsigned char *p_memory = (unsigned char *)malloc(malloc_size);
    if (p_memory == NULL) {
        return ERROR_MEM_ALLOC;
    }
    *p_p_image_data = p_memory;
    return SUCCESS;
}