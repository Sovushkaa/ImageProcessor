#pragma once
#include "filter.h"

class Negative : public Filter {
public:
    Negative();

    Image ApplyTo(const Image& image) const override;
};

struct NegativeFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParameters& params) const override;
};