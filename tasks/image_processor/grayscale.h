#pragma once
#include "filter.h"

class Grayscale : public Filter {
public:
    Grayscale();

    Image ApplyTo(const Image& image) const override;

    const float RED_IN_GRAYSCALE = 0.299;
    const float GREEN_IN_GRAYSCALE = 0.587;
    const float BLUE_IN_GRAYSCALE = 0.114;
};

struct GrayscaleFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParameters& params) const override;
};