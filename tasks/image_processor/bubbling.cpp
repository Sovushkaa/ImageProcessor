#include "bubbling.h"
#include <stdexcept>

Bubbling::Bubbling(size_t radius) : radius_(radius) {
}

Image Bubbling::ApplyTo(const Image& image) const {
    Image result = Image(image.width_, image.height_);
    for (size_t j = 0; j < image.height_; ++j) {
        for (size_t i = 0; i < image.width_; ++i) {
            Pixel cur_pixel = GetPixel(image, i, j);
            SetPixel(result, cur_pixel, i, j);
        }
    }
    size_t big_width = (image.width_ + 2 * radius_ - 1) / (2 * radius_);
    size_t big_height = (image.height_ + 2 * radius_ - 1) / (2 * radius_);
    std::vector<std::vector<Pixel> > curcle_colors;
    for (size_t i = 0; i < big_width; ++i) {
        std::vector<Pixel> cur_vector;
        for (size_t j = 0; j < big_height; ++j) {
            Pixel cur_color;
            cur_vector.push_back(cur_color);
        }
        curcle_colors.push_back(cur_vector);
    }
    size_t start_width = 0;
    size_t start_height = 0;
    for (size_t i = 0; i < big_width; ++i) {
        start_height = 0;
        for (size_t j = 0; j < big_height; ++j) {
            float cur_red = 0;
            float cur_green = 0;
            float cur_blue = 0;
            float cur_number = 0;
            size_t center_width = start_width + radius_;
            size_t center_height = start_height + radius_;
            for (size_t w = start_width; w < std::min(start_width + 2 * radius_, image.width_); ++w) {
                for (size_t h = start_height; h < std::min(start_height + 2 * radius_, image.height_); ++h) {
                    if ((w - center_width) * (w - center_width) + (h - center_height) * (h - center_height) <
                        radius_ * radius_) {
                        cur_number++;
                        Pixel cur_pixel = GetPixel(image, w, h);
                        cur_red += cur_pixel.red;
                        cur_blue += cur_pixel.blue;
                        cur_green += cur_pixel.green;
                    }
                }
            }
            cur_red /= cur_number;
            cur_green /= cur_number;
            cur_blue /= cur_number;
            curcle_colors[i][j].red = cur_red;
            curcle_colors[i][j].blue = cur_blue;
            curcle_colors[i][j].green = cur_green;
            start_height += 2 * radius_;
        }
        start_width += 2 * radius_;
    }
    start_width = 0;
    start_height = 0;
    for (size_t i = 0; i < big_width; ++i) {
        start_height = 0;
        for (size_t j = 0; j < big_height; ++j) {
            size_t center_width = start_width + radius_;
            size_t center_height = start_height + radius_;
            for (size_t w = start_width; w < std::min(start_width + 2 * radius_, image.width_); ++w) {
                for (size_t h = start_height; h < std::min(start_height + 2 * radius_, image.height_); ++h) {
                    if ((w - center_width) * (w - center_width) + (h - center_height) * (h - center_height) <
                        radius_ * radius_) {
                        SetPixel(result, curcle_colors[i][j], w, h);
                    }
                }
            }
            start_height += 2 * radius_;
        }
        start_width += 2 * radius_;
    }
    return result;
}

std::unique_ptr<Filter> BubblingFactory::Create(const FilterParameters& params) const {
    if (params.size() != 1) {
        throw std::runtime_error("Invalid arguments in bubbling");
    }
    size_t radius = std::stoi(params.at(0));
    return std::make_unique<Bubbling>(radius);
}