/* TODO:
 *  - Cleaning?
 *  - Better naming
 *  - Better comments
 *  - Remove todo list
 */

#ifndef SIFLIB_H
#define SIFLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Default file propeties
#define SIF_HEADER_SIZE         (11)
#define SIF_MAGIC_BYTES_SIZE    (3)
#define SIF_MAGIC_BYTES         ("SIF")


// Data struct that represents every image pixel
typedef struct _sifPixel
{
    /*
    * I decided to let it have Alpha support always. You can use 
    * 3rd party libraries to convert PNG, JPEG, JIFF... images
    * into Pixel buffers with alpha support (gimp has an "export
    * as c file" option which you can use)
    */

    uint8_t r; // Red
    uint8_t g; // Green
    uint8_t b; // Blue
    uint8_t a; // Alpha

} sifPixel;

// Data struct used to store external information about the image
typedef struct _sifHeader
{
    // Hexdump of an SIF file
    //00000000  53 49 46 00 00 00 00 00 |SIF.....|

    uint8_t magicBytes[SIF_MAGIC_BYTES_SIZE];

    // Size
    
    uint32_t x;
    uint32_t y;

} sifHeader;

// User data struct to manipulate
typedef struct _sifImage
{
    // Size

    uint32_t x;
    uint32_t y;

    // Pixel data
    
    sifPixel* data;

} sifImage;


// Functions

int32_t sifLoadImage(const char* path, sifImage** image);
int32_t sifUnloadImage(sifImage** image);
int32_t sifWriteImage(const char* path, sifImage** image);
int32_t sifAllocateImage(sifImage** image, uint32_t size_xy);
int32_t sifCreateImage(sifImage** image, uint32_t x, uint32_t y);


int32_t
sifLoadImage(const char* path, sifImage** image)
{
    FILE* file;
    uint8_t* fbuff;
    uint32_t size;

    // Open existing file for read & write

    file = fopen(path, "r");

    if(file == NULL)
    {
        fprintf(stderr, "[siflib] File didn't load correctly\n");
        return EXIT_FAILURE;
    }

    // Get file size

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    // Load file into memory
    
    fbuff = malloc(size);
    fread(fbuff, size, sizeof(uint8_t), file);
    
    // Detect SIF format
    
    if(memcmp(fbuff, SIF_MAGIC_BYTES, SIF_MAGIC_BYTES_SIZE))
    {
        fprintf(stderr, "[siflib] This is not a SIF file\n");
        free(fbuff);
        fclose(file);
        
        return EXIT_FAILURE;
    }

    // Allocate memory for the image struct

    sifAllocateImage(image, (size - SIF_HEADER_SIZE) / sizeof(sifPixel));

    // Copy file buffer into the image data

    (*image)->x = * (uint32_t*) (fbuff + SIF_MAGIC_BYTES_SIZE);
    (*image)->y = * (uint32_t*) (fbuff + SIF_MAGIC_BYTES_SIZE);
 
    memcpy((*image)->data, fbuff + SIF_HEADER_SIZE, size - SIF_HEADER_SIZE);

    // Cleaning

    free(fbuff);
    fclose(file);

    printf("[siflib] Image loaded (%dx%d)\n", (*image)->x, (*image)->y);

    return EXIT_SUCCESS;
}

int32_t
sifUnloadImage(sifImage** image)
{
    // Clean the heap

    free((*image)->data);
    free(*image);

    printf("[siflib] Image unloaded\n");

    return EXIT_SUCCESS;
}

int32_t
sifWriteImage(const char* path, sifImage** image)
{
    FILE* file;
    size_t size;

    // Open file as empty image

    file = fopen(path, "w");

    if (file == NULL)
    {
        fprintf(stderr, "[siflib] File couldn't be opened correctly\n");
        return EXIT_FAILURE;
    }
  
    // Dump to file
 
    fwrite(SIF_MAGIC_BYTES, SIF_MAGIC_BYTES_SIZE, 1, file);
    fwrite(*image, SIF_HEADER_SIZE - SIF_MAGIC_BYTES_SIZE, 1, file);
    fwrite((*image)->data, (*image)->x * (*image)->y * sizeof(sifPixel), 1, file);
    
    fclose(file);

    return EXIT_SUCCESS;

}

int32_t
sifAllocateImage(sifImage** image, uint32_t size_xy) {

    // Allocate heap to the image 
    // The first allocation contains the image dimensions & a pixel buffer pointer
    // The second one allocates the pixel buffer

    (*image)       = malloc(sizeof(sifImage));
    (*image)->data = malloc(sizeof(sifPixel) * size_xy); 

    return EXIT_SUCCESS;
}

int32_t
sifCreateImage(sifImage** image, uint32_t x, uint32_t y)
{
    // Used when not loading a image from file. Only user-side

    sifAllocateImage(image, x * y);
    (*image)->x = y;
    (*image)->y = x; 

    return EXIT_SUCCESS;
}

#endif /* SIFLIB_H */
