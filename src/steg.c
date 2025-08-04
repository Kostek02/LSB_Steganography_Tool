#include "../include/steg.h"

// Validate BMP format (24-bit, uncompressed)
int validate_bmp_format(FILE* file) {
    bmp_file_header_t file_header;
    bmp_info_header_t info_header;
    
    // Read file header
    if (fread(&file_header, sizeof(bmp_file_header_t), 1, file) != 1) {
        return STEG_FILE_ERROR;
    }
    
    // Check signature
    if (file_header.signature != BMP_SIGNATURE) {
        return STEG_INVALID_BMP;
    }
    
    // Read info header
    if (fread(&info_header, sizeof(bmp_info_header_t), 1, file) != 1) {
        return STEG_FILE_ERROR;
    }
    
    // Check if 24-bit
    if (info_header.bits_per_pixel != 24) {
        return STEG_INVALID_BMP;
    }
    
    // Check if uncompressed
    if (info_header.compression != 0) {
        return STEG_INVALID_BMP;
    }
    
    return STEG_SUCCESS;
}

// Calculate maximum message capacity
size_t calculate_message_capacity(FILE* file) {
    long current_pos = ftell(file);
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, current_pos, SEEK_SET);
    
    // Available bytes = file_size - header_size
    size_t available_bytes = file_size - BMP_HEADER_SIZE;
    
    // Each character needs 8 bytes (1 bit per byte)
    return available_bytes / 8;
}

// Read and validate BMP header
int read_bmp_header(FILE* file) {
    rewind(file);
    return validate_bmp_format(file);
}

// Write BMP header to output file
int write_bmp_header(FILE* input, FILE* output) {
    unsigned char header[BMP_HEADER_SIZE];
    
    rewind(input);
    if (fread(header, 1, BMP_HEADER_SIZE, input) != BMP_HEADER_SIZE) {
        return STEG_FILE_ERROR;
    }
    
    if (fwrite(header, 1, BMP_HEADER_SIZE, output) != BMP_HEADER_SIZE) {
        return STEG_FILE_ERROR;
    }
    
    return STEG_SUCCESS;
}

// Embed message into image using LSB steganography
int embed_message(const char* message, FILE* input, FILE* output) {
    if (!message || !input || !output) {
        return STEG_FILE_ERROR;
    }
    
    // Validate input BMP format
    int validation_result = read_bmp_header(input);
    if (validation_result != STEG_SUCCESS) {
        return validation_result;
    }
    
    // Calculate message capacity
    size_t capacity = calculate_message_capacity(input);
    size_t message_len = strlen(message);
    
    // Check if message fits (including null terminator)
    if (message_len + 1 > capacity) {
        return STEG_INSUFFICIENT_CAPACITY;
    }
    
    // Write header to output
    if (write_bmp_header(input, output) != STEG_SUCCESS) {
        return STEG_FILE_ERROR;
    }
    
    // Skip header in input file
    fseek(input, BMP_HEADER_SIZE, SEEK_SET);
    
    // Embed message bit by bit
    for (size_t i = 0; i <= message_len; i++) {  // Include null terminator
        char current_char = message[i];
        
        // Process each bit of the character
        for (int bit_pos = 7; bit_pos >= 0; bit_pos--) {
            int bit = (current_char >> bit_pos) & 1;
            
            // Read pixel byte
            unsigned char pixel_byte;
            if (fread(&pixel_byte, 1, 1, input) != 1) {
                return STEG_FILE_ERROR;
            }
            
            // Modify LSB: pixel_byte = (pixel_byte & 0xFE) | bit
            pixel_byte = (pixel_byte & 0xFE) | bit;
            
            // Write modified byte
            if (fwrite(&pixel_byte, 1, 1, output) != 1) {
                return STEG_FILE_ERROR;
            }
        }
    }
    
    // Copy remaining pixel data unchanged
    unsigned char remaining_byte;
    while (fread(&remaining_byte, 1, 1, input) == 1) {
        if (fwrite(&remaining_byte, 1, 1, output) != 1) {
            return STEG_FILE_ERROR;
        }
    }
    
    return STEG_SUCCESS;
}

// Extract message from image using LSB steganography
int extract_message(char* buffer, size_t max_len, FILE* input) {
    if (!buffer || !input || max_len == 0) {
        return STEG_FILE_ERROR;
    }
    
    // Validate input BMP format
    int validation_result = read_bmp_header(input);
    if (validation_result != STEG_SUCCESS) {
        return validation_result;
    }
    
    // Skip header
    fseek(input, BMP_HEADER_SIZE, SEEK_SET);
    
    size_t buffer_pos = 0;
    
    // Extract message character by character
    while (buffer_pos < max_len - 1) {
        char extracted_char = 0;
        
        // Extract 8 bits to form a character
        for (int bit_pos = 7; bit_pos >= 0; bit_pos--) {
            unsigned char pixel_byte;
            if (fread(&pixel_byte, 1, 1, input) != 1) {
                return STEG_FILE_ERROR;
            }
            
            // Extract LSB: bit = pixel_byte & 0x01
            int bit = pixel_byte & 1;
            
            // Build character: char = (bit7 << 7) | (bit6 << 6) | ... | bit0
            extracted_char |= (bit << bit_pos);
        }
        
        // Store character in buffer
        buffer[buffer_pos++] = extracted_char;
        
        // Check for null terminator
        if (extracted_char == '\0') {
            break;
        }
    }
    
    // Ensure null termination
    buffer[buffer_pos] = '\0';
    
    return STEG_SUCCESS;
}

// Print error messages
void print_error(int error_code) {
    switch (error_code) {
        case STEG_SUCCESS:
            printf("Operation completed successfully\n");
            break;
        case STEG_FILE_ERROR:
            fprintf(stderr, "Error: File I/O operation failed\n");
            break;
        case STEG_INVALID_BMP:
            fprintf(stderr, "Error: Invalid BMP format (must be 24-bit uncompressed)\n");
            break;
        case STEG_INSUFFICIENT_CAPACITY:
            fprintf(stderr, "Error: Image too small to hold the message\n");
            break;
        case STEG_MEMORY_ERROR:
            fprintf(stderr, "Error: Memory allocation failed\n");
            break;
        default:
            fprintf(stderr, "Error: Unknown error occurred\n");
            break;
    }
} 