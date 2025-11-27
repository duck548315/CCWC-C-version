#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <getopt.h>

#include "ccwc.h"

// Parse command-line arguments into Options struct.
Options parse_arguments(int argc, char* argv[]) {
    Options opts;

    static struct option long_options[] = {
        {"lines", no_argument, nullptr, 'l'},
        {"words", no_argument, nullptr, 'w'},
        {"chars", no_argument, nullptr, 'm'},
        {"bytes", no_argument, nullptr, 'c'},
        {"buffer_size", required_argument, nullptr, 'b'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "lwcmb:h", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'l': opts.count_lines = true; break;
            case 'w': opts.count_words = true; break;
            case 'c': opts.count_bytes = true; break;
            case 'm': opts.count_chars = true; break;
            case 'b': opts.buffer_size = std::stoul(optarg);
                break;
            case 'h':
                std::cout << "Usage: ccwc [-lwcm] [file...]" << std::endl;
                exit(0);
            default:
                exit(1);
        }
    }

    //Remaining arguments are filenames.
    for (int i = optind; i < argc; ++i) {
        opts.filenames.push_back(argv[i]);
    }

    // Default flags
    if (!opts.count_lines && !opts.count_words && 
        !opts.count_chars && !opts.count_bytes) {
        opts.count_lines = true;
        opts.count_words = true;
        opts.count_bytes = true;
    }
    
    return opts;
}

// Print counts for a single file/stream.
void print_counts(const CCWC::Counts& counts, const Options& opts, const std::string& filename = "") {
    if (opts.count_lines) std::cout << " " << counts.lines;
    if (opts.count_words) std::cout << " " << counts.words;
    if (opts.count_chars) std::cout << " " << counts.chars;
    if (opts.count_bytes) std::cout << " " << counts.bytes;

    if (!filename.empty()) {
        std::cout << " " << filename;
    }

    std::cout << std::endl;
}

// Process files and prints results.
void process_files(const Options& opts) {
    CCWC tool(opts.buffer_size);
    CCWC::Counts total = {0, 0, 0, 0};

    // Count how many flags are set
    int flag_count = opts.count_lines + opts.count_words + 
                     opts.count_chars + opts.count_bytes;

    // Read from stdin if no files specified.
    if (opts.filenames.empty()) {
        CCWC::Counts counts = run_count(tool, std::cin, opts, flag_count);
        print_counts(counts, opts);
        return;
    }

    // Process each file
    for (const auto& filename: opts.filenames) {
        std::ifstream file(filename, std::ios::binary);

        if (!file) {
            std::cerr << "Cannot open file'" << filename << "'" << std::endl;
            continue;
        }

        CCWC::Counts counts = run_count(tool, file, opts, flag_count);
        print_counts(counts, opts, filename);

        // Accumulate totals
        total.lines += counts.lines;
        total.words += counts.words;
        total.chars += counts.chars;
        total.bytes += counts.bytes;
    }

    // Print total if multiple files.
    if (opts.filenames.size() > 1) {
        print_counts(total, opts, "total");
    }
}

int main(int argc, char* argv[]) {
    Options opts = parse_arguments(argc, argv);
    process_files(opts);
    return 0;
}

// Single flag → use dedicated function, otherwise use count_all
CCWC::Counts run_count(CCWC& tool, std::istream& stream, 
                       const Options& opts, int flag_count) {
    CCWC::Counts counts = {0, 0, 0, 0};

    if (flag_count == 1) {
        // Single flag optimization
        if (opts.count_lines) counts.lines = tool.count_lines(stream);
        else if (opts.count_words) counts.words = tool.count_words(stream);
        else if (opts.count_chars) counts.chars = tool.count_chars(stream);
        else if (opts.count_bytes) counts.bytes = tool.count_bytes(stream);
    } 
    else {
        // Multiple flags → single pass
        counts = tool.count_all(stream);
    }

    return counts;
}