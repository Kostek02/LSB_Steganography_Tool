# LSB Steganography Tool Makefile
# Author: Konstanty Litwinow Jr.

# Directories
SRCDIR = src
INCDIR = include
DOCDIR = doc
WEBDIR = web
SCRIPTSDIR = scripts
BUILDDIR = build

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I$(INCDIR)
DEBUG_CFLAGS = -g -DDEBUG
RELEASE_CFLAGS = -O2

# Target executables
TARGET = steg
CLI_TARGET = steg_cli

# Source files
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/steg.c
CLI_SOURCES = $(SRCDIR)/steg_cli.c $(SRCDIR)/steg.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
CLI_OBJECTS = $(CLI_SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Create build directory
$(shell mkdir -p $(BUILDDIR))

# Default target
all: $(TARGET) $(CLI_TARGET)

# Debug build
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(TARGET) $(CLI_TARGET)

# Release build
release: CFLAGS += $(RELEASE_CFLAGS)
release: $(TARGET) $(CLI_TARGET)

# Build the demo executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# Build the CLI executable
$(CLI_TARGET): $(CLI_OBJECTS)
	$(CC) $(CLI_OBJECTS) -o $(CLI_TARGET)
	@echo "Build complete: $(CLI_TARGET)"

# Compile source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/steg.h
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(CLI_OBJECTS) $(TARGET) $(CLI_TARGET)
	rm -rf $(BUILDDIR)
	@echo "Clean complete"

# Clean everything (build artifacts + output files)
clean-all:
	rm -f $(OBJECTS) $(CLI_OBJECTS) $(TARGET) $(CLI_TARGET)
	rm -rf $(BUILDDIR)
	rm -f output.bmp cli_output.bmp demo_output.bmp *_output.bmp
	rm -f test_message.txt *.txt.bak
	@echo "Full cleanup complete"

# Install (copy to /usr/local/bin)
install: $(TARGET) $(CLI_TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	sudo cp $(CLI_TARGET) /usr/local/bin/
	@echo "Installed $(TARGET) and $(CLI_TARGET) to /usr/local/bin/"

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET) /usr/local/bin/$(CLI_TARGET)
	@echo "Uninstalled $(TARGET) and $(CLI_TARGET) from /usr/local/bin/"

# Run the demo program
run: $(TARGET)
	./$(TARGET)

# Run the CLI program
run-cli: $(CLI_TARGET)
	./$(CLI_TARGET) --help

# Test with a sample image (requires image.bmp)
test: $(TARGET)
	@if [ ! -f image.bmp ]; then \
		echo "Error: image.bmp not found. Please create a 24-bit BMP file for testing."; \
		echo "You can use GIMP, Photoshop, or online tools to create one."; \
		exit 1; \
	fi
	./$(TARGET)

# Test CLI with sample image
test-cli: $(CLI_TARGET)
	@if [ ! -f image.bmp ]; then \
		echo "Error: image.bmp not found. Please create a 24-bit BMP file for testing."; \
		echo "You can use GIMP, Photoshop, or online tools to create one."; \
		exit 1; \
	fi
	./$(CLI_TARGET) -c -i image.bmp

# Open web GUI
web: $(WEBDIR)/web_gui.html
	@echo "Opening web GUI..."
	@if command -v open &> /dev/null; then \
		open $(WEBDIR)/web_gui.html; \
	elif command -v xdg-open &> /dev/null; then \
		xdg-open $(WEBDIR)/web_gui.html; \
	else \
		echo "Please open $(WEBDIR)/web_gui.html in your web browser"; \
	fi

# Create test message file
test-message:
	@echo "Hello, World! This is a test message for the CLI tool." > test_message.txt
	@echo "Created test_message.txt"

# Demo CLI usage
demo-cli: $(CLI_TARGET) test-message
	@echo "=== CLI Demo ==="
	@echo "1. Check image capacity:"
	./$(CLI_TARGET) -c -i image.bmp
	@echo
	@echo "2. Embed message from file:"
	./$(CLI_TARGET) -e -f test_message.txt -i image.bmp -o demo_output.bmp -v
	@echo
	@echo "3. Extract message:"
	./$(CLI_TARGET) -x -i demo_output.bmp -v

# Run test setup script
setup: $(SCRIPTSDIR)/test_setup.sh
	@echo "Running test setup..."
	@chmod +x $(SCRIPTSDIR)/test_setup.sh
	@$(SCRIPTSDIR)/test_setup.sh

# Show project structure
tree:
	@echo "Project Structure:"
	@echo "=================="
	@echo "├── src/           # Source code files"
	@echo "│   ├── main.c     # Demo program"
	@echo "│   ├── steg.c     # Core steganography"
	@echo "│   └── steg_cli.c # CLI version"
	@echo "├── include/       # Header files"
	@echo "│   └── steg.h     # Main header"
	@echo "├── doc/           # Documentation"
	@echo "│   ├── dev_plan.txt # Development plan"
	@echo "│   ├── PROJECT_STRUCTURE.md # Structure guide"
	@echo "│   └── V1.0_RELEASE_NOTES.md # Release notes"
	@echo "├── web/           # Web interface"
	@echo "│   └── web_gui.html # Web GUI"
	@echo "├── scripts/       # Utility scripts"
	@echo "│   └── test_setup.sh # Test setup script"
	@echo "├── build/         # Build artifacts (auto-created)"
	@echo "├── Makefile       # Build system"
	@echo "├── README.md      # Main documentation"
	@echo "├── LICENSE        # MIT License"
	@echo "├── .gitignore     # Version control exclusions"
	@echo "└── image.bmp      # Test image"

# Show help
help:
	@echo "LSB Steganography Tool - Available targets:"
	@echo "  all        - Build both demo and CLI programs (default)"
	@echo "  debug      - Build with debug symbols"
	@echo "  release    - Build with optimization"
	@echo "  clean      - Remove build artifacts"
	@echo "  clean-all  - Remove build artifacts + output files"
	@echo "  install    - Install to /usr/local/bin"
	@echo "  uninstall  - Remove from /usr/local/bin"
	@echo "  run        - Build and run the demo program"
	@echo "  run-cli    - Build and show CLI help"
	@echo "  test       - Build and test demo with image.bmp"
	@echo "  test-cli   - Build and test CLI with image.bmp"
	@echo "  web        - Open web GUI in browser"
	@echo "  demo-cli   - Run full CLI demonstration"
	@echo "  setup      - Run test setup script"
	@echo "  tree       - Show project structure"
	@echo "  help       - Show this help message"

# Phony targets
.PHONY: all debug release clean clean-all install uninstall run run-cli test test-cli web test-message demo-cli setup tree help 