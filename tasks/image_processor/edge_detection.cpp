#include "image_processor.h"
#include "edge_detection.h"
#include "grayscale.h"

EdgeDetection::EdgeDetection(float threshold) : threshold_(threshold) {
}

Image EdgeDetection::ApplyTo(const Image& image) const {
    Image grayscaled_image = (*(grayscale_filter.get())).ApplyTo(image);
    Image detected_image = Image(image.width_, image.height_);
    for (size_t j = 0; j < grayscaled_image.height_; ++j) {
        for (size_t i = 0; i < grayscaled_image.width_; ++i) {
            Pixel cur_pixel = GetPixel(grayscaled_image, i, j);
            float value = 0;
            for (size_t row = 0; row <= 2; ++row) {
                for (size_t col = 0; col <= 2; ++col) {
                    if (WithinBorders(image, i + row - 1, j + col - 1)) {
                        value += matrix[row][col] * GetPixel(grayscaled_image, i + row - 1, j + col - 1).red;
                    } else {
                        value += matrix[row][col] * cur_pixel.red;
                    }
                }
            }
            if (value > threshold_) {
                SetPixel(detected_image, Pixel(1, 1, 1), i, j);
            } else {
                SetPixel(detected_image, Pixel(0, 0, 0), i, j);
            }
        }
    }
    return detected_image;
}

std::unique_ptr<Filter> EdgeDetectionFactory::Create(const FilterParameters& params) const {
    if (params.size() != 1) {
        throw std::runtime_error("Invalid arguments in crop");
    }
    float threshold = std::stof(params.at(0));
    return std::make_unique<EdgeDetection>(threshold);
}