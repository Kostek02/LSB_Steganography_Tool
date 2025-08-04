#include "../include/steg.h"

int main() {
    printf("LSB Steganography Tool\n");
    printf("======================\n\n");
    
    // Test message to embed
    const char* test_message = "Hello, World! This is a secret message hidden using LSB steganography.";
    
    printf("Test message: %s\n", test_message);
    printf("Message length: %zu characters\n\n", strlen(test_message));
    
    // Open input image
    FILE* input = fopen("image.bmp", "rb");
    if (!input) {
        fprintf(stderr, "Error: Could not open image.bmp\n");
        fprintf(stderr, "Please ensure you have a 24-bit BMP file named 'image.bmp' in the current directory.\n");
        return 1;
    }
    
    // Validate input image
    printf("Validating input image...\n");
    int validation_result = read_bmp_header(input);
    if (validation_result != STEG_SUCCESS) {
        print_error(validation_result);
        fclose(input);
        return 1;
    }
    
    // Calculate capacity
    size_t capacity = calculate_message_capacity(input);
    printf("Image capacity: %zu characters\n", capacity);
    
    if (strlen(test_message) + 1 > capacity) {
        fprintf(stderr, "Error: Message too long for this image\n");
        fprintf(stderr, "Required: %zu characters, Available: %zu characters\n", 
                strlen(test_message) + 1, capacity);
        fclose(input);
        return 1;
    }
    
    printf("✓ Image validation passed\n\n");
    
    // EMBED MESSAGE
    printf("=== EMBEDDING MESSAGE ===\n");
    
    FILE* output = fopen("output.bmp", "wb");
    if (!output) {
        fprintf(stderr, "Error: Could not create output.bmp\n");
        fclose(input);
        return 1;
    }
    
    int embed_result = embed_message(test_message, input, output);
    fclose(output);
    
    if (embed_result != STEG_SUCCESS) {
        print_error(embed_result);
        fclose(input);
        return 1;
    }
    
    printf("✓ Message embedded successfully\n");
    printf("✓ Output saved as 'output.bmp'\n\n");
    
    // EXTRACT MESSAGE
    printf("=== EXTRACTING MESSAGE ===\n");
    
    FILE* output_read = fopen("output.bmp", "rb");
    if (!output_read) {
        fprintf(stderr, "Error: Could not open output.bmp for reading\n");
        fclose(input);
        return 1;
    }
    
    char extracted_buffer[MAX_MESSAGE_LENGTH];
    int extract_result = extract_message(extracted_buffer, sizeof(extracted_buffer), output_read);
    fclose(output_read);
    
    if (extract_result != STEG_SUCCESS) {
        print_error(extract_result);
        fclose(input);
        return 1;
    }
    
    printf("✓ Message extracted successfully\n");
    printf("Extracted message: \"%s\"\n\n", extracted_buffer);
    
    // Verify extraction
    if (strcmp(test_message, extracted_buffer) == 0) {
        printf("✓ VERIFICATION PASSED: Original and extracted messages match!\n");
    } else {
        printf("✗ VERIFICATION FAILED: Messages do not match!\n");
        printf("Original:  \"%s\"\n", test_message);
        printf("Extracted: \"%s\"\n", extracted_buffer);
    }
    
    fclose(input);
    
    printf("\n=== SUMMARY ===\n");
    printf("• Input image: image.bmp\n");
    printf("• Output image: output.bmp (with hidden message)\n");
    printf("• Message capacity: %zu characters\n", capacity);
    printf("• Message length: %zu characters\n", strlen(test_message));
    printf("• Efficiency: %.1f%% of capacity used\n", 
           (double)(strlen(test_message) + 1) / capacity * 100);
    
    return 0;
} 