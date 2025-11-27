#ifndef CCWC_H
#define CCWC_H

#include <vector>
#include <string>
#include <istream>
#include <map>

class CCWC {
public:
    static constexpr size_t DEFAULT_BUFFER_SIZE = 64 * 1024;

    // Constructor
    explicit CCWC(size_t buffer_size = DEFAULT_BUFFER_SIZE);

    // Counting functions
    size_t count_bytes(std::istream& stream);
    size_t count_lines(std::istream& stream);
    size_t count_words(std::istream& stream);
    size_t count_chars(std::istream& stream);

    // Count all metrics in a single pass (optimized for stdin/pipes)
    struct Counts {
        size_t lines = 0;
        size_t words = 0;
        size_t chars = 0;
        size_t bytes = 0;
    };
    Counts count_all(std::istream& stream);

private:
    size_t buffer_size_;
};

// Command-line argument structure
struct Options {
    bool count_lines = false;
    bool count_words = false;
    bool count_chars = false;
    bool count_bytes = false;
    size_t buffer_size = CCWC::DEFAULT_BUFFER_SIZE;
    std::vector<std::string> filenames;
};

// Application functions
Options parse_arguments(int argc, char* argv[]);
void process_files(const Options& opts);
CCWC::Counts run_count(CCWC& tool, std::istream& stream, 
                       const Options& opts, int flag_count);

#endif // CCWC_H
