#pragma once
#include "filter.h"

class Bubbling : public Filter {
public:
    explicit Bubbling(size_t radius);

    Image ApplyTo(const Image& image) const override;
    size_t radius_;
};

struct BubblingFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParameters& params) const override;
};