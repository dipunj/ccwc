#include <iostream>
#include <cxxopts.hpp>
#include <fstream>
#include <string>
#include <filesystem>

cxxopts::ParseResult getCLIArgs(int argc, char** argv) {
    cxxopts::Options options("Word Count", "C++ implementation of the wc linux command");
    options.allow_unrecognised_options();
    options.add_options()
            ("L,maxlength", "Write the length of the line containing the "
                            "most "
                            "bytes "
                            "(default) more than one file argument is specified, the longest input line of all files is reported as the value of the final 'total'.")
            ("c,count", "The number of bytes in each input file is written "
                        "to the standard output.  This will cancel out any "
                        "prior usage of the -m option.", cxxopts::value<bool>()->default_value("true"))
            ("l,length", "The number of lines in each input file is written "
                         "to the"
                         " standard output.", cxxopts::value<bool>()->default_value("true"))
            ("m,characters", "The number of characters in each input file is "
                             "written to the standard output. If the current locale does not support multibyte characters, this is equivalent to the -c option.  This will cancel out any prior usage of the -c option.", cxxopts::value<bool>()->default_value("true"))
            ("w,words", "The number of words in each input file is written "
                        "to the standard output.", cxxopts::value<bool>()
                     ->default_value("true"));
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

    bool length = false;
    bool words = false;
    bool count = false;

    for (const auto& x:options.arguments()) {
        if (x.key() == "length" && x.value() == "true") {
            // call getLineCount function
            length = true;
        } else if (x.key() == "words" && x.value() == "true") {
            words = true;
        } else if (x.key() == "count" && x.value() == "true") {
            count = true;
        }
    }

    std::stringstream ss;
    if (filenames.empty()) {
        // read from stdin
        std::string std_input;
        while (getline(std::cin, std_input)) {
            ss << std_input << std::endl;
        }
    } else {

        for(const auto& file:filenames) {
            ss.clear();
            //line, word, byte, and file name.
            std::vector<std::string> result(4, "");

            ss = attachInput(file);
            std::string line, word;
            unsigned long lineCount = 0;
            unsigned long byteCount = 0;
            unsigned long wordCount = 0;

            while(std::getline(ss, line)) {
                lineCount++;
                byteCount += line.size();
                std::stringstream s_line(line);
                while(s_line >> word) {
                    wordCount++;
                }
            }

            if (length)
                std::cout << "   " << std::to_string(lineCount);
            if (words)
                std::cout << "   " << std::to_string(wordCount);
            if (count) {
                std::cout << "   " << std::to_string
                (std::filesystem::file_size(file));
            }

            std::cout << "    " << file << std::endl;
        }
    }

    return 0;
}
