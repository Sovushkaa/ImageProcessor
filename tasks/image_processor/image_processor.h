#include <algorithm>
#include <climits>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "image.h"
#include "filter.h"

#pragma once

std::vector<FilterArgs> Parse(std::vector<std::string> arguments);

void DoFilters(Image& image, std::vector<FilterArgs> parsed_args);