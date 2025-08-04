#include "../include/steg.h"
#include <getopt.h>

void print_usage(const char* program_name) {
    printf("LSB Steganography Tool - CLI Version\n");
    printf("====================================\n\n");
    printf("Usage: %s [OPTIONS]\n\n", program_name);
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
    printf("Examples:\n");
    printf("  %s -e -m \"Hello World\" -i photo.bmp -o secret.bmp\n", program_name);
    printf("  %s -x -i secret.bmp\n", program_name);
    printf("  %s -c -i photo.bmp\n", program_name);
    printf("  %s -e -f message.txt -i photo.bmp\n", program_name);
}

int read_message_from_file(const char* filename, char* buffer, size_t max_len) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open message file '%s'\n", filename);
        return -1;
    }
    
    size_t bytes_read = fread(buffer, 1, max_len - 1, file);
    buffer[bytes_read] = '\0';
    fclose(file);
    
    return 0;
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
    
    int option;
    int option_index = 0;
    
    while ((option = getopt_long(argc, argv, "exi:o:m:f:cvh", long_options, &option_index)) != -1) {
        switch (option) {
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
                print_usage(argv[0]);
                return 0;
            case '?':
                print_usage(argv[0]);
                return 1;
        }
    }
    
    // Validate mode selection
    int mode_count = embed_mode + extract_mode + capacity_mode;
    if (mode_count == 0) {
        fprintf(stderr, "Error: No mode specified. Use -e, -x, or -c\n\n");
        print_usage(argv[0]);
        return 1;
    }
    if (mode_count > 1) {
        fprintf(stderr, "Error: Multiple modes specified. Use only one of -e, -x, or -c\n\n");
        print_usage(argv[0]);
        return 1;
    }
    
    // Open input file
    FILE* input = fopen(input_file, "rb");
    if (!input) {
        fprintf(stderr, "Error: Could not open input file '%s'\n", input_file);
        return 1;
    }
    
    // Validate BMP format
    if (verbose) printf("Validating input image...\n");
    int validation_result = read_bmp_header(input);
    if (validation_result != STEG_SUCCESS) {
        print_error(validation_result);
        fclose(input);
        return 1;
    }
    
    // Calculate capacity
    size_t capacity = calculate_message_capacity(input);
    if (verbose) printf("Image capacity: %zu characters\n", capacity);
    
    // CAPACITY MODE
    if (capacity_mode) {
        printf("Image: %s\n", input_file);
        printf("Capacity: %zu characters\n", capacity);
        printf("File size: %ld bytes\n", ftell(input));
        fclose(input);
        return 0;
    }
    
    // EXTRACT MODE
    if (extract_mode) {
        if (verbose) printf("Extracting message from '%s'...\n", input_file);
        
        char buffer[MAX_MESSAGE_LENGTH];
        int extract_result = extract_message(buffer, sizeof(buffer), input);
        fclose(input);
        
        if (extract_result != STEG_SUCCESS) {
            print_error(extract_result);
            return 1;
        }
        
        printf("Extracted message: %s\n", buffer);
        return 0;
    }
    
    // EMBED MODE
    if (embed_mode) {
        char final_message[MAX_MESSAGE_LENGTH];
        
        // Get message from file if specified
        if (message_file) {
            if (read_message_from_file(message_file, final_message, sizeof(final_message)) != 0) {
                fclose(input);
                return 1;
            }
        } else if (message) {
            strncpy(final_message, message, sizeof(final_message) - 1);
            final_message[sizeof(final_message) - 1] = '\0';
        } else {
            fprintf(stderr, "Error: No message specified. Use -m or -f\n\n");
            print_usage(argv[0]);
            fclose(input);
            return 1;
        }
        
        size_t message_len = strlen(final_message);
        if (message_len + 1 > capacity) {
            fprintf(stderr, "Error: Message too long for this image\n");
            fprintf(stderr, "Required: %zu characters, Available: %zu characters\n", 
                    message_len + 1, capacity);
            fclose(input);
            return 1;
        }
        
        if (verbose) {
            printf("Embedding message: %s\n", final_message);
            printf("Message length: %zu characters\n", message_len);
        }
        
        // Open output file
        FILE* output = fopen(output_file, "wb");
        if (!output) {
            fprintf(stderr, "Error: Could not create output file '%s'\n", output_file);
            fclose(input);
            return 1;
        }
        
        // Embed message
        int embed_result = embed_message(final_message, input, output);
        fclose(output);
        fclose(input);
        
        if (embed_result != STEG_SUCCESS) {
            print_error(embed_result);
            return 1;
        }
        
        if (verbose) {
            printf("Message embedded successfully\n");
            printf("Output saved as: %s\n", output_file);
            printf("Efficiency: %.1f%% of capacity used\n", 
                   (double)(message_len + 1) / capacity * 100);
        } else {
            printf("Message embedded successfully into '%s'\n", output_file);
        }
        
        return 0;
    }
    
    fclose(input);
    return 0;
} 