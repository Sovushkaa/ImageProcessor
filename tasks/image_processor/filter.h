#include <vector>
#include <string>
#include <memory>
#include "image.h"

#pragma once

using FilterParameters = std::vector<std::string>;

struct FilterArgs {
    std::string name;
    FilterParameters parameters;
};

struct Args {
    std::string input_file;
    std::string output_file;
    std::vector<FilterArgs> filters;
};

class Filter {
public:
    virtual Image ApplyTo(const Image&) const = 0;
    virtual ~Filter() = default;
};

struct FilterFactory {
    virtual std::unique_ptr<Filter> Create(const FilterParameters& params) const = 0;
    virtual ~FilterFactory() = default;
};

bool WithinBorders(const Image& image, size_t i, size_t j);

void Update(float& value);