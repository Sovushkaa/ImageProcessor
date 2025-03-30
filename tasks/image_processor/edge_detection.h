#pragma once
#include "filter.h"
#include "grayscale.h"

class EdgeDetection : public Filter {
public:
    explicit EdgeDetection(float threshold);

    Image ApplyTo(const Image& image) const override;
    float threshold_;
    const float MIDDLE_VALUE = 4;
    GrayscaleFactory grayscale_factory;
    std::vector<std::string> params;
    std::unique_ptr<Filter> grayscale_filter = grayscale_factory.Create(params);

    std::vector<std::vector<float> > matrix = {{0, -1, 0}, {-1, MIDDLE_VALUE, -1}, {0, -1, 0}};
};

struct EdgeDetectionFactory : public FilterFactory {
    std::unique_ptr<Filter> Create(const FilterParameters& params) const override;
};