/**
 * @file steg_cli.c
 * @brief LSB Steganography Tool - Command Line Interface
 * @author Konstanty Litwinow Jr.
 * @version 1.1
 * @date 2025
 * 
 * Enhanced CLI with support for multiple image formats (BMP, PNG, JPEG).
 * Uses the format handler system for extensible format support.
 */

#include "../include/steg.h"
#include "../include/formats.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static void print_help(void) {
    printf("LSB Steganography Tool v1.1 - CLI Version\n");
    printf("==========================================\n\n");
    printf("Usage: %s [OPTIONS]\n\n", "steg_cli");
    
    printf("Modes:\n");
    printf("  -e, --embed <message>    Embed a message into an image\n");
    printf("  -x, --extract            Extract a message from an image\n\n");
    
    printf("Options:\n");
    printf("  -i, --input <file>       Input image file (default: image.bmp)\n");
    printf("  -o, --output <file>      Output image file (default: output.bmp)\n");
    printf("  -m, --message <text>     Message to embed (for embed mode)\n");
    printf("  -f, --file <file>        Read message from file (for embed mode)\n");
    printf("  -c, --capacity           Show image capacity without processing\n");
    printf("  -v, --verbose            Verbose output\n");
    printf("  -h, --help               Show this help message\n\n");
    
    printf("Supported Formats:\n");
    printf("  %s\n\n", get_supported_formats());
    
    printf("Examples:\n");
    printf("  %s -e -m \"Hello World\" -i photo.bmp -o secret.bmp\n", "steg_cli");
    printf("  %s -e -m \"Secret\" -i image.png -o hidden.png\n", "steg_cli");
    printf("  %s -x -i secret.jpg\n", "steg_cli");
    printf("  %s -c -i photo.png\n", "steg_cli");
    printf("  %s -e -f message.txt -i image.bmp\n", "steg_cli");
}

static void print_cli_error(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
}

static const char* get_error_message(int error_code) {
    switch (error_code) {
        case STEG_SUCCESS:
            return "Success";
        case STEG_FILE_ERROR:
            return "File operation failed";
        case STEG_INVALID_BMP:
            return "Invalid image format";
        case STEG_INSUFFICIENT_CAPACITY:
            return "Insufficient image capacity";
        case STEG_MEMORY_ERROR:
            return "Memory allocation failed";
        default:
            return "Unknown error";
    }
}

static int read_message_from_file(const char* filename, char* message, size_t max_len) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0;
    }
    
    size_t bytes_read = fread(message, 1, max_len - 1, file);
    message[bytes_read] = '\0';
    
    fclose(file);
    return 1;
}

int main(int argc, char* argv[]) {
    int embed_mode = 0;
    int extract_mode = 0;
    int capacity_mode = 0;
    int verbose = 0;
    
    char* input_file = "image.bmp";
    char* output_file = "output.bmp";
    char* message = NULL;
    char* message_file = NULL;
    
    // Long options
    static struct option long_options[] = {
        {"embed", no_argument, 0, 'e'},
        {"extract", no_argument, 0, 'x'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"message", required_argument, 0, 'm'},
        {"file", required_argument, 0, 'f'},
        {"capacity", no_argument, 0, 'c'},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int option_index = 0;
    int c;
    
    while ((c = getopt_long(argc, argv, "exi:o:m:f:cvh", long_options, &option_index)) != -1) {
        switch (c) {
            case 'e':
                embed_mode = 1;
                break;
            case 'x':
                extract_mode = 1;
                break;
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'm':
                message = optarg;
                break;
            case 'f':
                message_file = optarg;
                break;
            case 'c':
                capacity_mode = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
                print_help();
                return 0;
            case '?':
                print_help();
                return 1;
            default:
                abort();
        }
    }
    
    // Validate arguments
    if (!embed_mode && !extract_mode && !capacity_mode) {
        print_cli_error("Must specify a mode: embed (-e), extract (-x), or capacity (-c)");
        print_help();
        return 1;
    }
    
    if (embed_mode && extract_mode) {
        print_cli_error("Cannot specify both embed and extract modes");
        return 1;
    }
    
    if (embed_mode && !message && !message_file) {
        print_cli_error("Embed mode requires a message (-m) or message file (-f)");
        return 1;
    }
    
    if (message && message_file) {
        print_cli_error("Cannot specify both message (-m) and message file (-f)");
        return 1;
    }
    
    // Get format handler for input file
    format_handler_t* handler = get_format_handler(input_file);
    if (!handler) {
        print_cli_error("Unsupported file format. Supported formats:");
        fprintf(stderr, "  %s\n", get_supported_formats());
        return 1;
    }
    
    if (verbose) {
        printf("Using format handler: %s\n", handler->name);
    }
    
    // Open input file
    FILE* input = fopen(input_file, "rb");
    if (!input) {
        print_cli_error("Could not open input file");
        return 1;
    }
    
    // Validate file format
    if (!handler->validate(input)) {
        print_cli_error("Invalid file format");
        fclose(input);
        return 1;
    }
    
    // Handle capacity mode
    if (capacity_mode) {
        long capacity = handler->get_capacity(input);
        if (capacity < 0) {
            print_cli_error("Could not calculate capacity");
            fclose(input);
            return 1;
        }
        
        printf("Image: %s\n", input_file);
        printf("Format: %s\n", handler->name);
        printf("Capacity: %ld characters\n", capacity);
        
        fclose(input);
        return 0;
    }
    
    // Handle embed mode
    if (embed_mode) {
        char message_buffer[4096];
        
        // Get message from file if specified
        if (message_file) {
            if (!read_message_from_file(message_file, message_buffer, sizeof(message_buffer))) {
                print_cli_error("Could not read message file");
                fclose(input);
                return 1;
            }
            message = message_buffer;
        }
        
        if (verbose) {
            printf("Embedding message: \"%s\"\n", message);
            printf("Message length: %zu characters\n", strlen(message));
        }
        
        // Check capacity
        long capacity = handler->get_capacity(input);
        if (capacity < 0) {
            print_cli_error("Could not calculate capacity");
            fclose(input);
            return 1;
        }
        
        if (strlen(message) > (size_t)capacity) {
            print_cli_error("Message too long for image capacity");
            fclose(input);
            return 1;
        }
        
        // Open output file
        FILE* output = fopen(output_file, "wb");
        if (!output) {
            print_cli_error("Could not create output file");
            fclose(input);
            return 1;
        }
        
        // Embed message
        int result = handler->embed(input, output, message);
        
        fclose(input);
        fclose(output);
        
        if (result == STEG_SUCCESS) {
            if (verbose) {
                printf("✓ Message embedded successfully\n");
                printf("✓ Output saved as '%s'\n", output_file);
            }
        } else {
            print_cli_error("Failed to embed message");
            print_cli_error(get_error_message(result));
            return 1;
        }
    }
    
    // Handle extract mode
    if (extract_mode) {
        char extracted_message[4096];
        
        int result = handler->extract(input, extracted_message, sizeof(extracted_message));
        
        fclose(input);
        
        if (result == STEG_SUCCESS) {
            if (verbose) {
                printf("✓ Message extracted successfully\n");
            }
            printf("Extracted message: \"%s\"\n", extracted_message);
        } else {
            print_cli_error("Failed to extract message");
            print_cli_error(get_error_message(result));
            return 1;
        }
    }
    
    return 0;
} 