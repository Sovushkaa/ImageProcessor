#include "negative.h"
#include <stdexcept>

Negative::Negative() {
}

Image Negative::ApplyTo(const Image& image) const {
    Image negative_image = Image(image.width_, image.height_);
    for (size_t j = 0; j < image.height_; ++j) {
        for (size_t i = 0; i < image.width_; ++i) {
            Pixel cur_pixel = GetPixel(image, i, j);
            SetPixel(negative_image, Pixel(1 - cur_pixel.red, 1 - cur_pixel.green, 1 - cur_pixel.blue), i, j);
        }
    }
    return negative_image;
}

std::unique_ptr<Filter> NegativeFactory::Create(const FilterParameters& params) const {
    if (!params.empty()) {
        throw std::runtime_error("Invalid arguments in negative");
    }
    return std::make_unique<Negative>();
}