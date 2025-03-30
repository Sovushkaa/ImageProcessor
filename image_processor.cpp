#include "image_processor.h"
#include <algorithm>
#include <climits>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <memory>

int main(int argc, char** argv) {
    std::vector<std::string> arguments;
    for (size_t i = 0; i < argc; ++i) {
        arguments.emplace_back(argv[i]);
    }
    if (arguments.empty()) {
        throw std::runtime_error("Missing first argument");
    }
    if (arguments.size() < 2) {
        throw std::runtime_error("Missing second argument");
    }
    if (arguments[1].size() < 4 || arguments[1][arguments[1].size() - 4] != '.' ||
        arguments[1][arguments[1].size() - 3] != 'b' || arguments[1][arguments[1].size() - 2] != 'm' ||
        arguments[1][arguments[1].size() - 1] != 'p') {
        throw std::runtime_error("Invalid input file format");
    }
    std::string input_path = arguments[1];
    if (arguments.size() < 3) {
        throw std::runtime_error("Missing third argument");
    }
    if (arguments[2].size() < 4 || arguments[2][arguments[2].size() - 4] != '.' ||
        arguments[2][arguments[2].size() - 3] != 'b' || arguments[2][arguments[2].size() - 2] != 'm' ||
        arguments[2][arguments[2].size() - 1] != 'p') {
        throw std::runtime_error("Invalid input file format");
    }
    std::string output_path = arguments[2];
    Image image(0, 0);
    Read(image, input_path);
    std::vector<FilterArgs> parsed_args = Parse(arguments);
    DoFilters(image, parsed_args);
    Write(image, output_path);
    return 0;
}