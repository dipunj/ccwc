#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <filesystem>
#include <cxxopts.hpp>

// Function declarations
cxxopts::ParseResult getCLIArgs(int argc, char** argv);
std::stringstream attachInput(const std::string& filename);
void processFile(const std::string& filename, bool length, bool words, bool count);

cxxopts::ParseResult getCLIArgs(int argc, char** argv) {
    cxxopts::Options options("Word Count", "C++ implementation of the wc linux command");
    options.allow_unrecognised_options();
    options.add_options()
            ("L,maxlength", "Write the length of the line containing the most bytes", cxxopts::value<bool>()->default_value("false"))
            ("c,count", "The number of bytes in each input file is written to the standard output.", cxxopts::value<bool>()->default_value("false"))
            ("l,length", "The number of lines in each input file is written to the standard output.", cxxopts::value<bool>()->default_value("false"))
            ("m,characters", "The number of characters in each input file is written to the standard output. If the current locale does not support multibyte characters, this is equivalent to the -c option.", cxxopts::value<bool>()->default_value("false"))
            ("w,words", "The number of words in each input file is written to the standard output.", cxxopts::value<bool>()->default_value("false"));
    cxxopts::ParseResult result = options.parse(argc, argv);

    return result;
}


std::stringstream attachInput(const std::string& filename) {
    std::stringstream stream;
    std::filesystem::path filePath = std::filesystem::absolute(filename);
    std::filesystem::path cleanPath = std::filesystem::canonical(filePath);
    std::cout<<cleanPath<<std::endl;
    std::ifstream file(cleanPath);

    if (file.is_open()) {
        stream << file.rdbuf();
        file.close();
    } else {
        std::cerr<< "File not found"<<std::endl;
        exit(1);
    }

    return stream;
}

int main(int argc, char** argv) {
    auto options = getCLIArgs(argc, argv);
    std::vector<std::string> filenames = options.unmatched();

    bool length = options["length"].as<bool>();
    bool words = options["words"].as<bool>();
    bool count = options["count"].as<bool>();

    if (filenames.empty()) {
        // read from stdin
        std::string std_input;
        std::stringstream ss;
        while (getline(std::cin, std_input)) {
            ss << std_input << std::endl;
        }
        processFile("stdin", length, words, count);
    } else {
        for (const auto& file : filenames) {
            processFile(file, length, words, count);
        }
    }

    return 0;
}

void processFile(const std::string& filename, bool length, bool words, bool count) {
    std::stringstream ss = attachInput(filename);

    std::string line, word;
    unsigned long lineCount = 0;
    unsigned long byteCount = 0;
    unsigned long wordCount = 0;

    while (std::getline(ss, line)) {
        lineCount++;
        byteCount += line.size();
        std::stringstream s_line(line);
        wordCount += std::distance(std::istream_iterator<std::string>(s_line), std::istream_iterator<std::string>());
    }

    if (!length && !words && !count) {
        std::cout << "   " << lineCount << "    " << wordCount << "   " << std::filesystem::file_size(filename);
    } else {
        if (length)
            std::cout << "   " << lineCount;
        if (words)
            std::cout << "   " << wordCount;
        if (count)
            std::cout << "   " << std::filesystem::file_size(filename);
    }

    std::cout << "    " << filename << std::endl;
}