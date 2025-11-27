# ccwc - C++ Word Count

A C++ implementation of the Unix `wc` (word count) command-line tool.

## Description

`ccwc` counts bytes, lines, words, and characters in files or from standard input. This is a coding challenge project that reimplements the classic Unix utility with performance optimizations including buffered I/O and single-pass counting.

## Features

- Count bytes, lines, words, and characters
- Process multiple files with totals
- Read from standard input
- UTF-8 character counting support
- Optimized single-pass counting when multiple flags are used
- Configurable buffer size for performance tuning

## Building

### Requirements

- C++17 compatible compiler
- CMake 3.10 or higher


### Build Instructions
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```
The executable will be created in the project root directory as `ccwc`.
### Install (optional)
```bash
sudo cmake --install .
```

## Usage

```bash
ccwc [OPTIONS] [FILE...]
```

### Options

- `-c`, `--bytes` - Count bytes
- `-l`, `--lines` - Count lines
- `-w`, `--words` - Count words
- `-m`, `--chars` - Count characters (UTF-8 aware)
- `-b`, `--buffer_size SIZE` - Set buffer size for reading (default: 64KB)
- `-h`, `--help` - Display help message

If no options are specified, `-l`, `-w`, and `-c` are used by default.

### Examples

Count lines, words, and bytes in a file (default behavior):
```bash
./ccwc test.txt
```

Count only lines:
```bash
./ccwc -l test.txt
```

Count words and characters:
```bash
./ccwc -w -m test.txt
```

Process multiple files:
```bash
./ccwc file1.txt file2.txt file3.txt
```

Read from standard input:
```bash
cat test.txt | ./ccwc -l
```

## Performance

The implementation uses buffered I/O with a default 64KB buffer. When multiple counting options are specified, it performs a single-pass algorithm to avoid reading the file multiple times, which is especially efficient for standard input and pipes.

## Project Structure

```
.
├── include/
│   └── ccwc.h          # Header file with class and function declarations
├── src/
│   ├── ccwc.cpp        # Core counting logic implementation
│   └── main.cpp        # Command-line parsing and file processing
├── CMakeLists.txt      # Build configuration
└── README.md           # This file
```

## License

This is a coding challenge project.
