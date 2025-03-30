#pragma once
#include "filter.h"

class Crop : public Filter {
public:
    Crop(size_t width, size_t height);

    Image ApplyTo(const Image& image) const override;

    size_t new_width_;
    size_t new_height_;
};

struct CropFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParameters& params) const override;
};