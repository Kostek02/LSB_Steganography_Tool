# Project Structure

This document describes the organized folder structure of the LSB Steganography Tool.

## Directory Layout

```
lsb_steganography_tool/
├── 📁 src/                    # Source code files
│   ├── main.c                # Demo program entry point
│   ├── steg.c                # Core steganography implementation
│   └── steg_cli.c            # CLI version with command-line arguments
│
├── 📁 include/               # Header files
│   └── steg.h                # Main header with declarations and constants
│
├── 📁 doc/                   # Documentation
│   ├── dev_plan.txt          # Development roadmap and pla
│   ├── PROJECT_STRUCTURE.md  # This file
│   └── V1.0_RELEASE_NOTES.md # v1.0 release documentation
│
├── 📁 web/                   # Web interface
│   └── web_gui.html          # Modern web GUI with drag-and-drop
│
├── 📁 scripts/               # Utility scripts
│   └── test_setup.sh         # Automated test setup and cleanup
│
├── 📁 build/                 # Build artifacts (auto-created)
│   ├── *.o                   # Object files
│   └── (temporary build files)
│
├── 📄 Makefile               # Enhanced build system with multiple targets
├── 📄 README.md              # Main project documentation
├── 📄 LICENSE                # MIT License
├── 📄 .gitignore             # Version control exclusions
└── 🖼️ image.bmp              # Sample 24-bit BMP for testing
```

## Benefits of This Structure

### **Separation of Concerns**
- **Source code** is isolated in `src/`
- **Headers** are centralized in `include/`
- **Documentation** is organized in `doc/`
- **Web interface** is separate in `web/`
- **Scripts** are grouped in `scripts/`

### **Build System Improvements**
- **Automatic build directory** creation
- **Clean separation** of source and build artifacts
- **Proper include paths** with `-I$(INCDIR)`
- **Organized object files** in `build/` directory

### **Development Workflow**
- **Easy to find** specific file types
- **Scalable structure** for future additions
- **Clear organization** for new contributors
- **Professional layout** following C project conventions

## Build Commands

```bash
# Build everything
make

# Show project structure
make tree

# Clean build artifacts
make clean

# Clean everything including output files
make clean-all

# Run tests
make setup

# Open web GUI
make web
```

## Adding New Files

### **New Source Files**
- Place `.c` files in `src/`
- Update `Makefile` SOURCES variable
- Include headers with `#include "../include/steg.h"`

### **New Headers**
- Place `.h` files in `include/`
- Update `Makefile` if needed
- Include with `#include "header.h"` (due to `-I$(INCDIR)`)

### **New Documentation**
- Place in `doc/` directory
- Update this file if structure changes

### **New Scripts**
- Place in `scripts/` directory
- Make executable with `chmod +x`
- Add to `Makefile` if needed

## Version Control

The `.gitignore` file excludes:
- Build artifacts (`build/`, `*.o`, executables)
- Output files (`*_output.bmp`)
- Test files (`test_message.txt`)
- OS and IDE files

This ensures only source code and documentation are tracked. 