#include "ccwc.h"
#include <vector>


// Constructor
CCWC::CCWC(size_t buffer_size): buffer_size_(buffer_size) {}

size_t CCWC::count_bytes(std::istream& stream) {
    // Read all content and count bytes.
    std::vector<char> buffer(buffer_size_);
    size_t total = 0;

    while (stream.read(buffer.data(), buffer_size_) || stream.gcount() > 0) {
        total += stream.gcount();
    }

    return total;
}

size_t CCWC::count_lines(std::istream& stream) {
    std::vector<char> buffer(buffer_size_);
    size_t total = 0;

    while (stream.read(buffer.data(), buffer_size_) || stream.gcount() > 0) {
        size_t bytes_read = stream.gcount();
        for (size_t i = 0; i < bytes_read; ++i) {
            if (buffer[i] == '\n') {
                ++total;
            }
        }
    }

    return total;
}

size_t CCWC::count_words(std::istream& stream) {
    std::vector<char> buffer(buffer_size_);
    size_t total = 0;
    bool in_word = false;

    while (stream.read(buffer.data(), buffer_size_) || stream.gcount() > 0) {
        size_t bytes_read = stream.gcount();
        for (size_t i = 0; i < bytes_read; ++i) {
            // Check if current char is whitespace.
            bool is_space = (buffer[i] == ' ' || buffer[i] == '\r' || 
                             buffer[i] == '\t' || buffer[i] == '\n');
        
            // Entering a new word.
            if (!in_word && !is_space) {
                ++total;
                in_word = true;
            }
            else if (is_space) {
                in_word = false;
            }
        }
    }

    return total;
}

size_t CCWC::count_chars(std::istream& stream) {
    std::vector<char> buffer(buffer_size_);
    size_t total = 0;

    while ((stream.read(buffer.data(), buffer_size_) || stream.gcount() > 0)) {
        size_t bytes_read = stream.gcount();
        for (size_t i = 1; i < bytes_read; ++i) {
            unsigned char c = buffer[i];

            // Count only start bytes, skip continuation bytes (10xxxxxx)
            if ((c & 0xC0) != 0x80) {
                ++total;
            }
        }
    }

    return total;
}

CCWC::Counts CCWC::count_all(std::istream& stream) {
    std::vector<char> buffer(buffer_size_);
    CCWC::Counts counts = {0, 0, 0, 0};
    bool in_word = false;

    // Single pass: count everything at once.
    while (stream.read(buffer.data(), buffer_size_) || stream.gcount() > 0) {
        size_t bytes_read = stream.gcount();
        counts.bytes += bytes_read;

        for (size_t i = 0; i < bytes_read; ++i) {
            unsigned char c = buffer[i];

            // Lines
            if (c == '\n') {
                ++counts.lines;
            }

            // Words
            bool is_space = (c == ' ' || c == '\r' || c == '\t' || c == '\n');

            if (!in_word && !is_space) {
                ++counts.words;
                in_word = true;
            }
            else if (is_space) {
                in_word = false;
            }

            // Chars
            if ((c & 0xC0) != 0x80) {
                ++counts.chars;
            }
        }
    }

    return counts;

}