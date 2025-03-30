#include "grayscale.h"
#include <stdexcept>

Grayscale::Grayscale() {
}

Image Grayscale::ApplyTo(const Image& image) const {
    Image grayscaled_image = Image(image.width_, image.height_);
    for (size_t j = 0; j < image.height_; ++j) {
        for (size_t i = 0; i < image.width_; ++i) {
            Pixel cur_pixel = GetPixel(image, i, j);
            float new_value = RED_IN_GRAYSCALE * cur_pixel.red + GREEN_IN_GRAYSCALE * cur_pixel.green +
                              BLUE_IN_GRAYSCALE * cur_pixel.blue;
            SetPixel(grayscaled_image, Pixel(new_value, new_value, new_value), i, j);
        }
    }
    return grayscaled_image;
}

std::unique_ptr<Filter> GrayscaleFactory::Create(const FilterParameters& params) const {
    if (!params.empty()) {
        throw std::runtime_error("Invalid arguments in grayscale");
    }
    return std::make_unique<Grayscale>();
}