/**
 * @file formats.c
 * @brief Image Format Support - Implementation
 * @author Konstanty Litwinow Jr.
 * @version 1.1
 * @date 2025
 * 
 * Implementation of format handlers for multiple image formats.
 * Supports BMP, PNG, and JPEG with LSB steganography.
 */

#include "../include/formats.h"
#include "../include/steg.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h> // Required for strdup and free

// BMP format handler (existing implementation)
static int bmp_validate(FILE* file) {
    if (!file) return 0;
    rewind(file);
    return validate_bmp_format(file) == STEG_SUCCESS;
}

static long bmp_get_capacity(FILE* file) {
    if (!file) return -1;
    rewind(file);
    return calculate_message_capacity(file);
}

static int bmp_embed(FILE* input, FILE* output, const char* message) {
    if (!input || !output || !message) return STEG_FILE_ERROR;
    rewind(input);
    return embed_message(message, input, output);
}

static int bmp_extract(FILE* input, char* message, size_t max_len) {
    if (!input || !message) return STEG_FILE_ERROR;
    rewind(input);
    return extract_message(message, max_len, input);
}

// PNG format handler
static int png_validate(FILE* file) {
    unsigned char header[8];
    
    if (!file) return 0;
    
    // Read PNG signature
    if (fread(header, 1, 8, file) != 8) {
        return 0;
    }
    
    // PNG signature: 89 50 4E 47 0D 0A 1A 0A
    const unsigned char png_signature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    
    if (memcmp(header, png_signature, 8) != 0) {
        return 0;
    }
    
    rewind(file);
    return 1;
}

static long png_get_capacity(FILE* file) {
    // For PNG, we need to parse the IHDR chunk to get dimensions and color type
    // This is a more accurate implementation
    unsigned char buffer[32];
    long width, height;
    int color_type, bit_depth;
    
    if (!file) return -1;
    
    // Skip PNG signature (8 bytes)
    fseek(file, 8, SEEK_SET);
    
    // Read IHDR chunk header and data
    if (fread(buffer, 1, 25, file) != 25) {
        return -1;
    }
    
    // Extract width and height (big-endian)
    width = (buffer[4] << 24) | (buffer[5] << 16) | (buffer[6] << 8) | buffer[7];
    height = (buffer[8] << 24) | (buffer[9] << 16) | (buffer[10] << 8) | buffer[11];
    bit_depth = buffer[12];
    color_type = buffer[13];
    
    // Calculate capacity based on image dimensions and color type
    long capacity = 0;
    
    switch (color_type) {
        case 0: // Grayscale
            capacity = (width * height * bit_depth) / 8;
            break;
        case 2: // RGB
            capacity = (width * height * 3 * bit_depth) / 8;
            break;
        case 3: // Palette
            capacity = (width * height) / 8 + 256; // Add palette capacity
            break;
        case 4: // Grayscale + Alpha
            capacity = (width * height * 2 * bit_depth) / 8;
            break;
        case 6: // RGBA
            capacity = (width * height * 4 * bit_depth) / 8;
            break;
        default:
            capacity = (width * height * 3) / 8; // Default to RGB
            break;
    }
    
    // Ensure minimum capacity
    if (capacity < 10) {
        capacity = 10;
    }
    
    // Limit to reasonable size (max 1MB of data)
    if (capacity > 1000000) {
        capacity = 1000000;
    }
    
    rewind(file);
    return capacity;
}

static int png_embed(FILE* input, FILE* output, const char* message) {
    // PNG LSB steganography implementation
    // We'll embed the message in the IDAT chunk data
    
    unsigned char buffer[4096];
    size_t bytes_read;
    size_t message_len = strlen(message);
    size_t message_pos = 0;
    int in_idat = 0;
    unsigned char chunk_header[8];
    unsigned long chunk_length;
    char chunk_type[5];
    
    if (!input || !output || !message) {
        return STEG_FILE_ERROR;
    }
    
    rewind(input);
    
    // Copy PNG signature (8 bytes)
    if (fread(buffer, 1, 8, input) != 8) {
        return STEG_FILE_ERROR;
    }
    if (fwrite(buffer, 1, 8, output) != 8) {
        return STEG_FILE_ERROR;
    }
    
    // Process chunks
    while (fread(chunk_header, 1, 8, input) == 8) {
        // Extract chunk length and type
        chunk_length = (chunk_header[0] << 24) | (chunk_header[1] << 16) | 
                      (chunk_header[2] << 8) | chunk_header[3];
        memcpy(chunk_type, chunk_header + 4, 4);
        chunk_type[4] = '\0';
        
        // Write chunk header
        fwrite(chunk_header, 1, 8, output);
        
        // Check if this is IDAT chunk
        if (strcmp(chunk_type, "IDAT") == 0) {
            in_idat = 1;
            
            // Read and process IDAT data with LSB embedding
            size_t data_read = 0;
            while (data_read < chunk_length) {
                size_t to_read = (chunk_length - data_read > sizeof(buffer)) ? 
                                sizeof(buffer) : chunk_length - data_read;
                
                if (fread(buffer, 1, to_read, input) != to_read) {
                    return STEG_FILE_ERROR;
                }
                
                // Embed message bits in LSB of each byte
                for (size_t i = 0; i < to_read && message_pos < message_len * 8; i++) {
                    unsigned char bit = (message[message_pos / 8] >> (message_pos % 8)) & 1;
                    buffer[i] = (buffer[i] & 0xFE) | bit;
                    message_pos++;
                }
                
                if (fwrite(buffer, 1, to_read, output) != to_read) {
                    return STEG_FILE_ERROR;
                }
                data_read += to_read;
            }
        } else {
            // Copy other chunks as-is
            size_t data_read = 0;
            while (data_read < chunk_length) {
                size_t to_read = (chunk_length - data_read > sizeof(buffer)) ? 
                                sizeof(buffer) : chunk_length - data_read;
                
                if (fread(buffer, 1, to_read, input) != to_read) {
                    return STEG_FILE_ERROR;
                }
                if (fwrite(buffer, 1, to_read, output) != to_read) {
                    return STEG_FILE_ERROR;
                }
                data_read += to_read;
            }
        }
        
        // Read and copy CRC
        if (fread(buffer, 1, 4, input) != 4) {
            return STEG_FILE_ERROR;
        }
        if (fwrite(buffer, 1, 4, output) != 4) {
            return STEG_FILE_ERROR;
        }
        
        // Stop after IEND chunk
        if (strcmp(chunk_type, "IEND") == 0) {
            break;
        }
    }
    
    return STEG_SUCCESS;
}

static int png_extract(FILE* input, char* message, size_t max_len) {
    // PNG LSB steganography extraction
    // Extract message from IDAT chunk data
    
    unsigned char buffer[4096];
    unsigned char chunk_header[8];
    unsigned long chunk_length;
    char chunk_type[5];
    size_t message_pos = 0;
    int in_idat = 0;
    
    if (!input || !message) {
        return STEG_FILE_ERROR;
    }
    
    rewind(input);
    
    // Skip PNG signature (8 bytes)
    if (fseek(input, 8, SEEK_SET) != 0) {
        return STEG_FILE_ERROR;
    }
    
    // Process chunks
    while (fread(chunk_header, 1, 8, input) == 8) {
        // Extract chunk length and type
        chunk_length = (chunk_header[0] << 24) | (chunk_header[1] << 16) | 
                      (chunk_header[2] << 8) | chunk_header[3];
        memcpy(chunk_type, chunk_header + 4, 4);
        chunk_type[4] = '\0';
        
        // Check if this is IDAT chunk
        if (strcmp(chunk_type, "IDAT") == 0) {
            in_idat = 1;
            
            // Read and extract message from IDAT data
            size_t data_read = 0;
            unsigned char current_byte = 0;
            int bit_count = 0;
            
            while (data_read < chunk_length && message_pos < max_len - 1) {
                size_t to_read = (chunk_length - data_read > sizeof(buffer)) ? 
                                sizeof(buffer) : chunk_length - data_read;
                
                if (fread(buffer, 1, to_read, input) != to_read) {
                    return STEG_FILE_ERROR;
                }
                
                // Extract message bits from LSB of each byte
                for (size_t i = 0; i < to_read && message_pos < max_len - 1; i++) {
                    unsigned char bit = buffer[i] & 1;
                    current_byte = (current_byte << 1) | bit;
                    bit_count++;
                    
                    if (bit_count == 8) {
                        message[message_pos] = current_byte;
                        message_pos++;
                        current_byte = 0;
                        bit_count = 0;
                        
                        // Check for null terminator
                        if (current_byte == 0) {
                            message[message_pos] = '\0';
                            return STEG_SUCCESS;
                        }
                    }
                }
                data_read += to_read;
            }
        } else {
            // Skip other chunks
            if (fseek(input, chunk_length + 4, SEEK_CUR) != 0) {
                return STEG_FILE_ERROR;
            }
        }
        
        // Stop after IEND chunk
        if (strcmp(chunk_type, "IEND") == 0) {
            break;
        }
    }
    
    message[message_pos] = '\0';
    return STEG_SUCCESS;
}

// JPEG format handler
static int jpeg_validate(FILE* file) {
    unsigned char header[2];
    
    if (!file) return 0;
    
    // Read JPEG signature
    if (fread(header, 1, 2, file) != 2) {
        return 0;
    }
    
    // JPEG signature: FF D8
    if (header[0] != 0xFF || header[1] != 0xD8) {
        return 0;
    }
    
    rewind(file);
    return 1;
}

static long jpeg_get_capacity(FILE* file) {
    // JPEG capacity calculation is complex due to compression
    // This is a simplified implementation
    
    long file_size;
    
    if (!file) return -1;
    
    // Get file size
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    
    // Estimate capacity (simplified - about 1/10 of file size)
    long capacity = file_size / 10;
    
    return capacity;
}

static int jpeg_embed(FILE* input, FILE* output, const char* message) {
    // JPEG LSB steganography implementation
    // We'll embed the message in the image data after DCT coefficients
    
    unsigned char buffer[4096];
    size_t bytes_read;
    size_t message_len = strlen(message);
    size_t message_pos = 0;
    int in_image_data = 0;
    unsigned char marker[2];
    
    if (!input || !output || !message) {
        return STEG_FILE_ERROR;
    }
    
    rewind(input);
    
    // Copy JPEG signature (2 bytes)
    if (fread(marker, 1, 2, input) != 2) {
        return STEG_FILE_ERROR;
    }
    if (fwrite(marker, 1, 2, output) != 2) {
        return STEG_FILE_ERROR;
    }
    
    // Process JPEG segments
    while (fread(marker, 1, 2, input) == 2) {
        // Check for JPEG markers
        if (marker[0] == 0xFF) {
            unsigned char marker_type = marker[1];
            
            // Write marker
            fwrite(marker, 1, 2, output);
            
            // Handle different marker types
            if (marker_type == 0xD8) { // SOI
                continue;
            } else if (marker_type == 0xD9) { // EOI
                break;
            } else if (marker_type == 0xDA) { // SOS - Start of Scan
                in_image_data = 1;
                
                // Read segment length
                unsigned char length_bytes[2];
                if (fread(length_bytes, 1, 2, input) != 2) {
                    return STEG_FILE_ERROR;
                }
                unsigned int segment_length = (length_bytes[0] << 8) | length_bytes[1];
                
                // Write segment length
                fwrite(length_bytes, 1, 2, output);
                
                // Copy segment data
                for (unsigned int i = 2; i < segment_length; i++) {
                    unsigned char byte;
                    if (fread(&byte, 1, 1, input) != 1) {
                        return STEG_FILE_ERROR;
                    }
                    fwrite(&byte, 1, 1, output);
                }
                
                // Now process image data with LSB embedding
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), input)) > 0) {
                    // Check for end of image data (next marker)
                    for (size_t i = 0; i < bytes_read - 1; i++) {
                        if (buffer[i] == 0xFF && buffer[i + 1] != 0x00) {
                            // Found a marker, write remaining data and stop
                            if (i > 0) {
                                // Embed message in remaining data
                                for (size_t j = 0; j < i && message_pos < message_len * 8; j++) {
                                    unsigned char bit = (message[message_pos / 8] >> (message_pos % 8)) & 1;
                                    buffer[j] = (buffer[j] & 0xFE) | bit;
                                    message_pos++;
                                }
                                fwrite(buffer, 1, i, output);
                            }
                            
                            // Write the marker
                            fwrite(&buffer[i], 1, 2, output);
                            
                            // Copy remaining data
                            if (i + 2 < bytes_read) {
                                fwrite(&buffer[i + 2], 1, bytes_read - i - 2, output);
                            }
                            
                            // Continue with next segment
                            goto next_segment;
                        }
                    }
                    
                    // Embed message in this buffer
                    for (size_t i = 0; i < bytes_read && message_pos < message_len * 8; i++) {
                        unsigned char bit = (message[message_pos / 8] >> (message_pos % 8)) & 1;
                        buffer[i] = (buffer[i] & 0xFE) | bit;
                        message_pos++;
                    }
                    
                    fwrite(buffer, 1, bytes_read, output);
                }
                
                next_segment:
                continue;
            } else {
                // Other markers - copy segment
                unsigned char length_bytes[2];
                if (fread(length_bytes, 1, 2, input) != 2) {
                    return STEG_FILE_ERROR;
                }
                unsigned int segment_length = (length_bytes[0] << 8) | length_bytes[1];
                
                fwrite(length_bytes, 1, 2, output);
                
                // Copy segment data
                for (unsigned int i = 2; i < segment_length; i++) {
                    unsigned char byte;
                    if (fread(&byte, 1, 1, input) != 1) {
                        return STEG_FILE_ERROR;
                    }
                    fwrite(&byte, 1, 1, output);
                }
            }
        } else {
            // Not a marker, copy as-is
            fwrite(marker, 1, 2, output);
        }
    }
    
    return STEG_SUCCESS;
}

static int jpeg_extract(FILE* input, char* message, size_t max_len) {
    // JPEG LSB steganography extraction
    // Extract message from image data
    
    unsigned char buffer[4096];
    size_t bytes_read;
    size_t message_pos = 0;
    int in_image_data = 0;
    unsigned char marker[2];
    unsigned char current_byte = 0;
    int bit_count = 0;
    
    if (!input || !message) {
        return STEG_FILE_ERROR;
    }
    
    rewind(input);
    
    // Skip JPEG signature (2 bytes)
    if (fseek(input, 2, SEEK_SET) != 0) {
        return STEG_FILE_ERROR;
    }
    
    // Process JPEG segments
    while (fread(marker, 1, 2, input) == 2) {
        // Check for JPEG markers
        if (marker[0] == 0xFF) {
            unsigned char marker_type = marker[1];
            
            // Handle different marker types
            if (marker_type == 0xD8) { // SOI
                continue;
            } else if (marker_type == 0xD9) { // EOI
                break;
            } else if (marker_type == 0xDA) { // SOS - Start of Scan
                in_image_data = 1;
                
                // Skip segment length
                if (fseek(input, 2, SEEK_CUR) != 0) {
                    return STEG_FILE_ERROR;
                }
                
                // Read segment data length
                unsigned char length_bytes[2];
                if (fread(length_bytes, 1, 2, input) != 2) {
                    return STEG_FILE_ERROR;
                }
                unsigned int segment_length = (length_bytes[0] << 8) | length_bytes[1];
                
                // Skip segment data
                if (fseek(input, segment_length - 2, SEEK_CUR) != 0) {
                    return STEG_FILE_ERROR;
                }
                
                // Now extract message from image data
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), input)) > 0) {
                    // Check for end of image data (next marker)
                    for (size_t i = 0; i < bytes_read - 1; i++) {
                        if (buffer[i] == 0xFF && buffer[i + 1] != 0x00) {
                            // Found a marker, extract from remaining data and stop
                            if (i > 0) {
                                // Extract message from remaining data
                                for (size_t j = 0; j < i && message_pos < max_len - 1; j++) {
                                    unsigned char bit = buffer[j] & 1;
                                    current_byte = (current_byte << 1) | bit;
                                    bit_count++;
                                    
                                    if (bit_count == 8) {
                                        message[message_pos] = current_byte;
                                        message_pos++;
                                        current_byte = 0;
                                        bit_count = 0;
                                        
                                        // Check for null terminator
                                        if (current_byte == 0) {
                                            message[message_pos] = '\0';
                                            return STEG_SUCCESS;
                                        }
                                    }
                                }
                            }
                            
                            // Continue with next segment
                            goto next_segment;
                        }
                    }
                    
                    // Extract message from this buffer
                    for (size_t i = 0; i < bytes_read && message_pos < max_len - 1; i++) {
                        unsigned char bit = buffer[i] & 1;
                        current_byte = (current_byte << 1) | bit;
                        bit_count++;
                        
                        if (bit_count == 8) {
                            message[message_pos] = current_byte;
                            message_pos++;
                            current_byte = 0;
                            bit_count = 0;
                            
                            // Check for null terminator
                            if (current_byte == 0) {
                                message[message_pos] = '\0';
                                return STEG_SUCCESS;
                            }
                        }
                    }
                }
                
                next_segment:
                continue;
            } else {
                // Other markers - skip segment
                unsigned char length_bytes[2];
                if (fread(length_bytes, 1, 2, input) != 2) {
                    return STEG_FILE_ERROR;
                }
                unsigned int segment_length = (length_bytes[0] << 8) | length_bytes[1];
                
                if (fseek(input, segment_length - 2, SEEK_CUR) != 0) {
                    return STEG_FILE_ERROR;
                }
            }
        } else {
            // Not a marker, skip
            if (fseek(input, -1, SEEK_CUR) != 0) {
                return STEG_FILE_ERROR;
            }
        }
    }
    
    message[message_pos] = '\0';
    return STEG_SUCCESS;
}

// Format handler instances
format_handler_t bmp_handler = {
    .name = "BMP",
    .extensions = ".bmp,.BMP",
    .validate = bmp_validate,
    .get_capacity = bmp_get_capacity,
    .embed = bmp_embed,
    .extract = bmp_extract
};

format_handler_t png_handler = {
    .name = "PNG",
    .extensions = ".png,.PNG",
    .validate = png_validate,
    .get_capacity = png_get_capacity,
    .embed = png_embed,
    .extract = png_extract
};

format_handler_t jpeg_handler = {
    .name = "JPEG",
    .extensions = ".jpg,.jpeg,.JPG,.JPEG",
    .validate = jpeg_validate,
    .get_capacity = jpeg_get_capacity,
    .embed = jpeg_embed,
    .extract = jpeg_extract
};

// Array of all handlers
static format_handler_t* handlers[] = {
    &bmp_handler,
    &png_handler,
    &jpeg_handler,
    NULL
};

/**
 * @brief Get format handler for a given filename
 * 
 * @param filename The filename to get handler for
 * @return Pointer to format handler or NULL if not supported
 */
format_handler_t* get_format_handler(const char* filename) {
    if (!filename) return NULL;
    
    // Get file extension
    const char* ext = strrchr(filename, '.');
    if (!ext) return NULL;
    
    // Check each handler
    for (int i = 0; handlers[i] != NULL; i++) {
        const char* supported_exts = handlers[i]->extensions;
        char* ext_copy = strdup(supported_exts);
        if (!ext_copy) continue;
        
        char* token = strtok(ext_copy, ",");
        while (token != NULL) {
            // Skip leading whitespace
            while (*token == ' ') token++;
            
            if (strcasecmp(ext, token) == 0) {
                free(ext_copy);
                return handlers[i];
            }
            token = strtok(NULL, ",");
        }
        
        free(ext_copy);
    }
    
    return NULL;
}

/**
 * @brief Get list of supported formats
 * 
 * @return String containing supported format names
 */
const char* get_supported_formats(void) {
    static char formats[256] = "";
    int pos = 0;
    
    for (int i = 0; handlers[i] != NULL; i++) {
        if (pos > 0) {
            pos += snprintf(formats + pos, sizeof(formats) - pos, ", ");
        }
        pos += snprintf(formats + pos, sizeof(formats) - pos, "%s", handlers[i]->name);
    }
    
    return formats;
}

/**
 * @brief Check if a format is supported
 * 
 * @param filename The filename to check
 * @return 1 if supported, 0 if not
 */
int is_format_supported(const char* filename) {
    return get_format_handler(filename) != NULL;
} 