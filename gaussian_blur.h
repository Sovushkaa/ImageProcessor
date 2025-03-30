#pragma once
#include "filter.h"

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(float sigma);

    Image ApplyTo(const Image& image) const override;

    float sigma;
};

struct GaussianBlurFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParameters& params) const override;
};