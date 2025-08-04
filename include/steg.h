/**
 * @file steg.h
 * @brief LSB Steganography Tool - Main Header File
 * @author Konstanty Litwinow Jr.
 * @version 1.0
 * @date 2025
 * 
 * This header file contains all declarations, constants, and structures
 * for the LSB (Least Significant Bit) steganography tool.
 * 
 * The tool supports hiding and extracting ASCII text messages in 24-bit BMP images
 * by modifying the least significant bit of each pixel byte.
 */

#ifndef STEG_H
#define STEG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// ============================================================================
// CONSTANTS
// ============================================================================

/** @brief Size of BMP file header in bytes */
#define BMP_HEADER_SIZE 54

/** @brief Maximum length of message that can be embedded/extracted */
#define MAX_MESSAGE_LENGTH 4096

/** @brief BMP file signature ("BM" in little-endian) */
#define BMP_SIGNATURE 0x4D42

// ============================================================================
// ERROR CODES
// ============================================================================

/** @brief Operation completed successfully */
#define STEG_SUCCESS 0

/** @brief File I/O operation failed */
#define STEG_FILE_ERROR -1

/** @brief Invalid BMP format (must be 24-bit uncompressed) */
#define STEG_INVALID_BMP -2

/** @brief Image too small to hold the message */
#define STEG_INSUFFICIENT_CAPACITY -3

/** @brief Memory allocation failed */
#define STEG_MEMORY_ERROR -4

// ============================================================================
// BMP FILE STRUCTURES
// ============================================================================

/**
 * @brief BMP file header structure (14 bytes)
 * 
 * This structure contains the basic file information including
 * the signature, file size, and offset to pixel data.
 */
typedef struct {
    uint16_t signature;      /**< "BM" signature (0x4D42) */
    uint32_t file_size;      /**< Total file size in bytes */
    uint16_t reserved1;      /**< Reserved field (must be 0) */
    uint16_t reserved2;      /**< Reserved field (must be 0) */
    uint32_t data_offset;    /**< Offset to pixel data from file start */
} __attribute__((packed)) bmp_file_header_t;

/**
 * @brief BMP info header structure (40 bytes)
 * 
 * This structure contains detailed image information including
 * dimensions, color depth, and compression settings.
 */
typedef struct {
    uint32_t header_size;    /**< Size of info header (40 bytes) */
    int32_t width;          /**< Image width in pixels */
    int32_t height;         /**< Image height in pixels */
    uint16_t planes;        /**< Number of color planes (must be 1) */
    uint16_t bits_per_pixel; /**< Bits per pixel (must be 24) */
    uint32_t compression;   /**< Compression type (0 = uncompressed) */
    uint32_t image_size;    /**< Size of pixel data in bytes */
    int32_t x_pixels_per_m; /**< Horizontal resolution (pixels/meter) */
    int32_t y_pixels_per_m; /**< Vertical resolution (pixels/meter) */
    uint32_t colors_used;   /**< Number of colors in palette (0 = all) */
    uint32_t important_colors; /**< Important colors (0 = all) */
} __attribute__((packed)) bmp_info_header_t;

// ============================================================================
// CORE STEGANOGRAPHY FUNCTIONS
// ============================================================================

/**
 * @brief Embed a message into a BMP image using LSB steganography
 * 
 * @param message The ASCII text message to embed
 * @param input Input BMP file stream
 * @param output Output BMP file stream
 * @return Error code (STEG_SUCCESS on success)
 * 
 * This function hides the given message in the least significant bits
 * of the pixel data. Each character requires 8 pixel bytes (1 bit per byte).
 * The message is automatically null-terminated.
 */
int embed_message(const char* message, FILE* input, FILE* output);

/**
 * @brief Extract a hidden message from a BMP image
 * 
 * @param buffer Output buffer to store the extracted message
 * @param max_len Maximum length of the buffer
 * @param input Input BMP file stream
 * @return Error code (STEG_SUCCESS on success)
 * 
 * This function reads the least significant bits of pixel data
 * and reconstructs the hidden message. Extraction stops at the
 * first null terminator encountered.
 */
int extract_message(char* buffer, size_t max_len, FILE* input);

/**
 * @brief Read and validate BMP file header
 * 
 * @param file Input file stream
 * @return Error code (STEG_SUCCESS if valid 24-bit BMP)
 * 
 * Validates that the file is a proper 24-bit uncompressed BMP image.
 */
int read_bmp_header(FILE* file);

/**
 * @brief Write BMP header to output file
 * 
 * @param input Source file stream
 * @param output Destination file stream
 * @return Error code (STEG_SUCCESS on success)
 * 
 * Copies the 54-byte BMP header from input to output file.
 */
int write_bmp_header(FILE* input, FILE* output);

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

/**
 * @brief Validate BMP format requirements
 * 
 * @param file Input file stream
 * @return Error code (STEG_SUCCESS if valid)
 * 
 * Checks that the file is a 24-bit uncompressed BMP image.
 * This function reads both file and info headers.
 */
int validate_bmp_format(FILE* file);

/**
 * @brief Calculate maximum message capacity
 * 
 * @param file Input file stream
 * @return Maximum number of characters that can be embedded
 * 
 * Calculates how many characters can be hidden in the image
 * based on available pixel data (file_size - header_size) / 8.
 */
size_t calculate_message_capacity(FILE* file);

/**
 * @brief Print error message for given error code
 * 
 * @param error_code Error code from steganography functions
 * 
 * Displays a human-readable error message for the given error code.
 */
void print_error(int error_code);

#endif // STEG_H 