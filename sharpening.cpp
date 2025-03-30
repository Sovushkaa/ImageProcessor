#include "sharpening.h"
#include <stdexcept>

bool WithinBorders(const Image& image, size_t i, size_t j) {
    if (i < 0 || i >= image.width_) {
        return false;
    }
    if (j < 0 || j >= image.height_) {
        return false;
    }
    return true;
}

void Update(float& value) {
    if (value < 0) {
        value = 0;
    }
    if (value > 1) {
        value = 1;
    }
}

Sharpening::Sharpening() {
}

Image Sharpening::ApplyTo(const Image& image) const {
    Image sharped_image = Image(image.width_, image.height_);
    for (size_t j = 0; j < image.height_; ++j) {
        for (size_t i = 0; i < image.width_; ++i) {
            Pixel cur_pixel = GetPixel(image, i, j);
            float red_value = 0;
            for (size_t row = 0; row <= 2; ++row) {
                for (size_t col = 0; col <= 2; ++col) {
                    if (WithinBorders(image, i + row - 1, j + col - 1)) {
                        red_value += matrix[row][col] * GetPixel(image, i + row - 1, j + col - 1).red;
                    } else {
                        red_value += matrix[row][col] * cur_pixel.red;
                    }
                }
            }
            float green_value = 0;
            for (size_t row = 0; row <= 2; ++row) {
                for (size_t col = 0; col <= 2; ++col) {
                    if (WithinBorders(image, i + row - 1, j + col - 1)) {
                        green_value += matrix[row][col] * GetPixel(image, i + row - 1, j + col - 1).green;
                    } else {
                        green_value += matrix[row][col] * cur_pixel.green;
                    }
                }
            }
            float blue_value = 0;
            for (size_t row = 0; row <= 2; ++row) {
                for (size_t col = 0; col <= 2; ++col) {
                    if (WithinBorders(image, i + row - 1, j + col - 1)) {
                        blue_value += matrix[row][col] * GetPixel(image, i + row - 1, j + col - 1).blue;
                    } else {
                        blue_value += matrix[row][col] * cur_pixel.blue;
                    }
                }
            }
            Update(red_value);
            Update(green_value);
            Update(blue_value);

            SetPixel(sharped_image, Pixel(red_value, green_value, blue_value), i, j);
        }
    }
    return sharped_image;
}

std::unique_ptr<Filter> SharpeningFactory::Create(const FilterParameters& params) const {
    if (!params.empty()) {
        throw std::runtime_error("Invalid arguments in sharpening");
    }
    return std::make_unique<Sharpening>();
}