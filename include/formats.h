/**
 * @file formats.h
 * @brief Image Format Support - Header File
 * @author Konstanty Litwinow Jr.
 * @version 1.1
 * @date 2025
 * 
 * This header file defines the format handler interface and structures
 * for supporting multiple image formats in the LSB steganography tool.
 * 
 * Supported formats:
 * - BMP (24-bit uncompressed)
 * - PNG (lossless)
 * - JPEG (lossy, with careful handling)
 */

#ifndef FORMATS_H
#define FORMATS_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

// Format handler function pointer types
typedef int (*format_validate_func)(FILE* file);
typedef long (*format_get_capacity_func)(FILE* file);
typedef int (*format_embed_func)(FILE* input, FILE* output, const char* message);
typedef int (*format_extract_func)(FILE* input, char* message, size_t max_len);

/**
 * @brief Format handler structure
 * 
 * Defines the interface for image format handlers.
 * Each supported format must implement these functions.
 */
typedef struct {
    const char* name;                    ///< Format name (e.g., "BMP", "PNG", "JPEG")
    const char* extensions;              ///< File extensions (e.g., ".bmp,.BMP")
    format_validate_func validate;       ///< Validate file format
    format_get_capacity_func get_capacity; ///< Calculate message capacity
    format_embed_func embed;             ///< Embed message in image
    format_extract_func extract;         ///< Extract message from image
} format_handler_t;

// Format handler functions
format_handler_t* get_format_handler(const char* filename);
const char* get_supported_formats(void);
int is_format_supported(const char* filename);

// Individual format handlers
extern format_handler_t bmp_handler;
extern format_handler_t png_handler;
extern format_handler_t jpeg_handler;

// Format-specific constants
#define MAX_FORMAT_NAME 16
#define MAX_EXTENSIONS 64
#define MAX_HANDLERS 10

#endif // FORMATS_H 