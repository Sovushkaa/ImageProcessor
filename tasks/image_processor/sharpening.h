#pragma once
#include "filter.h"

class Sharpening : public Filter {
public:
    Sharpening();

    Image ApplyTo(const Image& image) const override;

    const float MIDDLE_VALUE = 5;

    std::vector<std::vector<float> > matrix = {{0, -1, 0}, {-1, MIDDLE_VALUE, -1}, {0, -1, 0}};
};

struct SharpeningFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParameters& params) const override;
};