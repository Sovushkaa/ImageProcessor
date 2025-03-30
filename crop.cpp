#include "crop.h"
#include <stdexcept>

Crop::Crop(size_t width, size_t height) : new_width_(width), new_height_(height) {
}

Image Crop::ApplyTo(const Image& image) const {
    if (new_width_ <= 0 || new_height_ <= 0) {
        throw std::runtime_error("Negative measurement");
    }
    size_t new_width = new_width_;
    size_t new_height = new_height_;
    if (new_width > image.width_) {
        new_width = image.width_;
    }
    if (new_height > image.height_) {
        new_height = image.height_;
    }
    Image cropped_image = Image(new_width, new_height);
    for (size_t j = 0; j < new_height; ++j) {
        for (size_t i = 0; i < new_width; ++i) {
            SetPixel(cropped_image, GetPixel(image, i, image.height_ - j - 1), i, new_height - j - 1);
        }
    }
    return cropped_image;
}

std::unique_ptr<Filter> CropFactory::Create(const FilterParameters& params) const {
    if (params.size() != 2) {
        throw std::runtime_error("Invalid arguments in crop");
    }
    size_t width = std::stoi(params.at(0));
    size_t height = std::stoi(params.at(1));
    return std::make_unique<Crop>(width, height);
}