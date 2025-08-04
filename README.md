# 🔐 LSB Steganography Tool v1.0

[![Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Standard](https://img.shields.io/badge/Standard-C99-green.svg)](https://en.wikipedia.org/wiki/C99)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Version](https://img.shields.io/badge/Version-1.0-orange.svg)]()

A comprehensive educational tool for learning and practicing LSB (Least Significant Bit) steganography in C. This project demonstrates how to hide and extract secret messages in 24-bit BMP images using bit manipulation techniques.

## 🚀 Quick Start

```bash
# Clone the repository
git clone <repository-url>
cd lsb_steganography_tool

# Build the project
make

# Run the demo
./steg

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
│   └── steg_cli.c   # CLI version with arguments
├── 📁 include/      # Header files
│   └── steg.h       # Main header with declarations
├── 📁 doc/          # Documentation
│   ├── dev_plan.txt # Development roadmap
│   ├── PROJECT_STRUCTURE.md # Structure guide
│   └── V1.0_RELEASE_NOTES.md # Release notes
├── 📁 web/          # Web interface
│   └── web_gui.html # Modern web GUI
├── 📁 scripts/      # Utility scripts
│   └── test_setup.sh # Test setup and cleanup
├── 📁 build/        # Build artifacts (auto-created)
├── 📄 Makefile      # Enhanced build system
├── 📄 README.md     # This file - Main documentation
├── 📄 LICENSE       # MIT License
├── 📄 .gitignore    # Version control exclusions
└── 🖼️ image.bmp     # Sample 24-bit BMP for testing
```

## 🛠️ Features

### **Core Functionality**
- ✅ **LSB Steganography** - Hide messages in 24-bit BMP images
- ✅ **Message Embedding** - Convert text to bits and modify pixel LSBs
- ✅ **Message Extraction** - Read LSBs and reconstruct hidden text
- ✅ **BMP Validation** - Ensure proper 24-bit uncompressed format
- ✅ **Capacity Calculation** - Determine maximum message size

### **Multiple Interfaces**
- 🖥️ **Demo Program** (`steg`) - Simple demonstration with predefined message
- 💻 **CLI Tool** (`steg_cli`) - Full command-line interface with options
- 🌐 **Web GUI** (`web_gui.html`) - Modern browser-based interface

### **Build System**
- 🔧 **Makefile** - Comprehensive build system with multiple targets
- 🧹 **Cleanup** - Automatic cleanup of build artifacts and output files
- 📦 **Installation** - Easy installation to system directories

## 📖 Usage

### **Demo Program**
```bash
# Build and run demo
make run

# The demo will:
# 1. Load image.bmp
# 2. Embed a test message
# 3. Create output.bmp
# 4. Extract and verify the message
```

### **CLI Tool**
```bash
# Check image capacity
./steg_cli -c -i image.bmp

# Embed a message
./steg_cli -e -m "Secret message" -i image.bmp -o secret.bmp -v

# Extract a message
./steg_cli -x -i secret.bmp -v

# Embed from file
./steg_cli -e -f message.txt -i image.bmp -o output.bmp
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
```

## 📚 Documentation

- **[Project Structure](doc/PROJECT_STRUCTURE.md)** - Detailed folder organization guide
- **[Release Notes](doc/V1.0_RELEASE_NOTES.md)** - v1.0 release documentation
- **

## 🧪 Testing

### **Quick Test**
```bash
# Automated test setup
make setup

# Manual testing
make test      # Test demo program
make test-cli  # Test CLI functionality
```

### **Test Requirements**
- 24-bit uncompressed BMP image named `image.bmp`
- Minimum size: 100x100 pixels recommended
- The tool will create test images if ImageMagick or Python3 is available

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
**Version:** 1.0  
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