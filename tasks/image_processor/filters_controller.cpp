#include "image_processor.h"
#include "crop.h"
#include "grayscale.h"
#include "negative.h"
#include "sharpening.h"
#include "gaussian_blur.h"
#include "edge_detection.h"
#include "bubbling.h"
#include <map>

std::vector<FilterArgs> Parse(std::vector<std::string> arguments) {
    std::vector<FilterArgs> result;
    for (size_t i = 3; i < arguments.size(); ++i) {
        if (arguments[i][0] == '-') {
            FilterArgs filter_args;
            filter_args.name = arguments[i];
            result.push_back(filter_args);
        } else {
            if (result.empty()) {
                throw std::runtime_error("Invalid arguments");
            }
            result[result.size() - 1].parameters.push_back(arguments[i]);
        }
    }
    return result;
}

void DoFilters(Image& image, std::vector<FilterArgs> parsed_args) {
    std::map<std::string, std::unique_ptr<FilterFactory>> fabrics;
    fabrics["-crop"] = std::make_unique<CropFactory>();
    fabrics["-gs"] = std::make_unique<GrayscaleFactory>();
    fabrics["-neg"] = std::make_unique<NegativeFactory>();
    fabrics["-sharp"] = std::make_unique<SharpeningFactory>();
    fabrics["-edge"] = std::make_unique<EdgeDetectionFactory>();
    fabrics["-bubble"] = std::make_unique<BubblingFactory>();
    fabrics["-blur"] = std::make_unique<GaussianBlurFactory>();
    for (const auto& el : parsed_args) {
        if (fabrics.find(el.name) == fabrics.end()) {
            throw std::runtime_error("Invalid filter name");
        }
        std::unique_ptr<Filter> f = fabrics.at(el.name)->Create(el.parameters);
        image = (*(f.get())).ApplyTo(image);
    }
}