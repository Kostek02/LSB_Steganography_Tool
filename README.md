# 🔐 LSB Steganography Tool v1.1

[![Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Standard](https://img.shields.io/badge/Standard-C99-green.svg)](https://en.wikipedia.org/wiki/C99)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Version](https://img.shields.io/badge/Version-1.1-orange.svg)]()

A comprehensive educational tool for learning and practicing LSB (Least Significant Bit) steganography in C. This project demonstrates how to hide and extract secret messages in multiple image formats using bit manipulation techniques.

## 🚀 Quick Start

```bash
# Clone the repository
git clone https://github.com/Kostek02/LSB_Steganography_Tool
cd lsb_steganography_tool

# Build the project
make

# Test with sample files
./steg_cli -x -i samples/test_bmp_with_message.bmp

# Use the CLI tool
./steg_cli --help

# Open the web GUI
make web
```

## 📁 Project Structure

```
lsb_steganography_tool/
├── 📁 src/           # Source code files
│   ├── main.c       # Demo program
│   ├── steg.c       # Core steganography implementation
│   ├── steg_cli.c   # CLI version with arguments
│   └── formats.c    # Multi-format support
├── 📁 include/      # Header files
│   ├── steg.h       # Main header with declarations
│   └── formats.h    # Format handler interface
├── 📁 doc/          # Documentation
│   ├── PROJECT_STRUCTURE.md # Structure guide
│   ├── V1.0_RELEASE_NOTES.md # Release notes
│   ├── V1.1_DEVELOPMENT_PLAN.md # v1.1 roadmap
│   ├── V1.1_FORMAT_SUPPORT_IMPLEMENTATION.md # Format implementation
│   └── V1.1_FINAL_IMPLEMENTATION_SUMMARY.md # Final summary
├── 📁 samples/      # Sample images and test files
│   ├── README.md    # Samples documentation
│   ├── image.bmp    # Original test image
│   ├── sample.bmp   # Sample BMP image
│   ├── sample.png   # Sample PNG image
│   ├── sample.jpg   # Sample JPEG image
│   ├── test_bmp_with_message.bmp    # BMP with hidden message
│   ├── test_png_with_message.png    # PNG with hidden message
│   └── test_jpeg_with_message.jpg   # JPEG with hidden message
├── 📁 web/          # Web interface
│   └── web_gui.html # Modern web GUI
├── 📁 scripts/      # Utility scripts
│   └── test_setup.sh # Test setup and cleanup
├── 📁 build/        # Build artifacts (auto-created)
├── 📄 Makefile      # Enhanced build system
├── 📄 README.md     # This file - Main documentation
├── 📄 LICENSE       # MIT License
├── 📄 .gitignore    # Version control exclusions
└── 🖼️ samples/      # Sample images and test files
```

## ✨ Features

- **Multi-Format Support**: BMP (24-bit), PNG (lossless), and JPEG (lossy) formats
- **LSB Steganography**: Hide and extract ASCII messages using Least Significant Bit technique
- **High Capacity**: Support for large messages depending on image size
- **Command Line Interface**: Full-featured CLI with comprehensive options
- **Web GUI**: Modern, responsive web interface for easy use
- **Sample Files**: Ready-to-use test images for all supported formats
- **Cross-Platform**: Works on Windows, macOS, and Linux

## 📖 Usage

### **Quick Testing with Sample Files**
```bash
# Extract hidden message from BMP test file
./steg_cli -x -i samples/test_bmp_with_message.bmp

# Check capacity of sample images
./steg_cli -c -i samples/sample.bmp
./steg_cli -c -i samples/sample.png
./steg_cli -c -i samples/sample.jpg

# Hide your own message in sample images
./steg_cli -e -m "Your secret message" -i samples/sample.bmp -o my_output.bmp
```

### **Demo Program**
```bash
# Build and run demo
make run

# The demo will:
# 1. Load samples/image.bmp
# 2. Embed a test message
# 3. Create output.bmp
# 4. Extract and verify the message
```

### **CLI Tool**
```bash
# Check image capacity
./steg_cli -c -i samples/sample.bmp

# Embed a message
./steg_cli -e -m "Secret message" -i samples/sample.bmp -o secret.bmp -v

# Extract a message
./steg_cli -x -i secret.bmp -v

# Embed from file
./steg_cli -e -f message.txt -i samples/sample.bmp -o output.bmp
```

### **Web GUI**
```bash
# Open in browser
make web

# Features:
# - Drag-and-drop file upload
# - Real-time image preview
# - Capacity estimation
# - Interactive embedding/extraction
```

## 🔧 Build Commands

```bash
# Build everything
make

# Debug build
make debug

# Release build
make release

# Clean build artifacts
make clean

# Clean everything (including output files)
make clean-all

# Install to system
make install

# Show project structure
make tree

# Run test setup
make setup

# Test multi-format support
make test-formats
```

## 📚 Documentation

- **[Development Plan](doc/V1.1_DEVELOPMENT_PLAN.md)** - Project roadmap and future features
- **[Project Structure](doc/PROJECT_STRUCTURE.md)** - Detailed folder organization guide
- **[Release Notes](doc/V1.0_RELEASE_NOTES.md)** - v1.0 release documentation
- **[Format Implementation](doc/V1.1_FORMAT_SUPPORT_IMPLEMENTATION.md)** - Multi-format support details
- **[Final Summary](doc/V1.1_FINAL_IMPLEMENTATION_SUMMARY.md)** - v1.1 implementation summary
- **[Samples Guide](samples/README.md)** - Sample images and test files documentation

## 🧪 Testing

### **Quick Test with Samples**
```bash
# Test message extraction
./steg_cli -x -i samples/test_bmp_with_message.bmp

# Test format detection
./steg_cli -c -i samples/sample.bmp
./steg_cli -c -i samples/sample.png
./steg_cli -c -i samples/sample.jpg

# Test multi-format support
make test-formats
```

### **Test Requirements**
- Sample images are provided in the `samples/` directory
- Test files with hidden messages are included
- The tool will create test images if needed

## 🔒 Security Considerations

**⚠️ Important Disclaimer:**
This tool is designed for **educational purposes only**. The implementation demonstrates basic steganography concepts and is not suitable for secure communication.

**Limitations:**
- LSB steganography is easily detectable by statistical analysis
- No encryption or obfuscation is implemented
- Use only for learning and ethical testing

## 🛠️ Development

### **Adding New Features**
1. Place source files in `src/`
2. Add headers to `include/`
3. Update `Makefile` if needed
4. Document changes in `doc/`

### **Code Style**
- Follow C99 standard
- Use meaningful variable names
- Include comprehensive comments
- Add error handling

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

**Author:** Konstanty Litwinow Jr. (@kljr.dev)  
**Version:** 1.1.0  
**Year:** 2025

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Implement your changes
4. Add tests if applicable
5. Submit a pull request

## 📞 Support

For questions, issues, or contributions:
- Create an issue on the project repository
- Contact the author directly
- Refer to the documentation in `doc/`

---

**Happy Steganography Learning! 🔐✨** 