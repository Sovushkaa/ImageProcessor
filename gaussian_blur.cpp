#include "gaussian_blur.h"
#include <stdexcept>
#include <cmath>

GaussianBlur::GaussianBlur(float sigma) : sigma(sigma) {
}

const int RADIUS = 3;

Image GaussianBlur::ApplyTo(const Image& image) const {
    Image blured_image = Image(image.width_, image.height_);
    std::vector<std::vector<Pixel> > cur_result;
    for (size_t i = 0; i < image.width_; ++i) {
        std::vector<Pixel> cur_vector;
        for (size_t j = 0; j < image.height_; ++j) {
            cur_vector.push_back(Pixel(0, 0, 0));
        }
        cur_result.push_back(cur_vector);
    }
    for (size_t it = 0; it < 2; ++it) {
        for (size_t j = 0; j < image.height_; ++j) {
            for (size_t i = 0; i < image.width_; ++i) {
                if (it == 1) {
                    cur_result[i][j] = Pixel(0, 0, 0);
                }
                float exp_sum = 0;
                size_t left = 0;
                size_t right = 0;
                if (it == 0) {
                    left = std::max(0, static_cast<int>(static_cast<float>(j) - RADIUS * sigma));
                    right = std::min(image.height_, static_cast<size_t>(static_cast<float>(j) + RADIUS * sigma));
                } else {
                    left = std::max(0, static_cast<int>(static_cast<float>(j) - RADIUS * sigma));
                    right = std::min(image.height_, static_cast<size_t>(static_cast<float>(j) + RADIUS * sigma));
                }
                for (size_t k = left; k < right; ++k) {
                    if (it == 0) {
                        float cur_exp = std::exp(-((static_cast<float>((j - k) * (j - k)))) / (2 * sigma * sigma));
                        cur_result[i][j].red += GetPixel(image, i, k).red * cur_exp;
                        cur_result[i][j].green += GetPixel(image, i, k).green * cur_exp;
                        cur_result[i][j].blue += GetPixel(image, i, k).blue * cur_exp;
                        exp_sum += cur_exp;
                    } else {
                        float cur_exp = std::exp(-((static_cast<float>((i - k) * (i - k))) / (2 * sigma * sigma)));
                        cur_result[i][j].red += GetPixel(blured_image, k, j).red * cur_exp;
                        cur_result[i][j].green += GetPixel(blured_image, k, j).green * cur_exp;
                        cur_result[i][j].blue += GetPixel(blured_image, k, j).blue * cur_exp;
                        exp_sum += cur_exp;
                    }
                }
                cur_result[i][j].red /= exp_sum;
                cur_result[i][j].green /= exp_sum;
                cur_result[i][j].blue /= exp_sum;
            }
        }
        if (it == 0) {
            for (size_t j = 0; j < image.height_; ++j) {
                for (size_t i = 0; i < image.width_; ++i) {
                    SetPixel(blured_image, cur_result[i][j], i, j);
                }
            }
        } else {
            for (size_t j = 0; j < image.height_; ++j) {
                for (size_t i = 0; i < image.width_; ++i) {
                    SetPixel(blured_image, cur_result[i][j], i, j);
                }
            }
        }
    }
    return blured_image;
}

std::unique_ptr<Filter> GaussianBlurFactory::Create(const FilterParameters& params) const {
    if (params.size() != 1) {
        throw std::runtime_error("Invalid arguments in blur");
    }
    float sigma = std::stof(params.at(0));
    return std::make_unique<GaussianBlur>(sigma);
}