#include "image.h"
#include <fstream>
#include <stdexcept>

const float MAX_COLOR_VALUE = 255.0f;

Pixel::Pixel() : red(0), green(0), blue(0) {
}

Pixel::Pixel(Color red, Color green, Color blue) : red(red), green(green), blue(blue) {
}

Image::Image(size_t width, size_t height) : width_(width), height_(height) {
    if (width < 0 || height < 0) {
        throw std::runtime_error("Negative measurement");
    }
    pixels_ = std::vector<Pixel>(width_ * height_);
}

const Pixel& GetPixel(const Image& image, size_t i, size_t j) {
    return image.pixels_[j * image.width_ + i];
}

void SetPixel(Image& image, const Pixel& color, size_t i, size_t j) {
    image.pixels_.at(j * image.width_ + i) = color;
}
void BytesSplit(unsigned char* v, size_t x) {
    v[0] = x;
    v[1] = x >> CHAR_BIT_SIZE;
    v[2] = x >> CHAR_BIT_SIZE * 2;
    v[3] = x >> CHAR_BIT_SIZE * 3;
}
int BytesMerge(unsigned char* v, size_t index) {
    return v[index] + (v[index + 1] << CHAR_BIT_SIZE) + (v[index + 2] << CHAR_BIT_SIZE * 2) +
           (v[index + 3] << CHAR_BIT_SIZE * 3);
}

const int SHIFT_IN_FIRST_HEADER = 8;

FirstHeader::FirstHeader() {
    std::fill(zero_bytes, zero_bytes + SHIFT_IN_FIRST_HEADER, 0);
}

FirstHeader::FirstHeader(const int total_size) {
    BytesSplit(size_bytes, total_size);
    std::fill(zero_bytes, zero_bytes + SHIFT_IN_FIRST_HEADER, 0);
}

const int SHIFT_IN_LAST_ZERO_BYTES = 25;

SecondHeader::SecondHeader() {
    std::fill(zero_bytes, zero_bytes + 3, 0);
    std::fill(last_zero_bytes, last_zero_bytes + SHIFT_IN_LAST_ZERO_BYTES, 0);
}

SecondHeader::SecondHeader(size_t width, size_t height, size_t second_header_size) {
    size_byte[0] = second_header_size;
    std::fill(zero_bytes, zero_bytes + 3, 0);
    BytesSplit(width_bytes, width);
    BytesSplit(height_bytes, height);
    std::fill(last_zero_bytes, last_zero_bytes + SHIFT_IN_LAST_ZERO_BYTES, 0);
}

BMPHeader::BMPHeader() {
}

BMPHeader::BMPHeader(Image image) {
    remainder = static_cast<const int>((image.width_ * 3) % 4);
    padding = ((4 - remainder) % 4);
    total_size = static_cast<const int>(first_header_size + second_header_size + image.width_ * image.height_ * 3 +
                                        padding * image.height_);
    first_header = FirstHeader(total_size);
    second_header = SecondHeader(image.width_, image.height_, second_header_size);
}

void Write(const Image& image, std::string path) {
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);
    BMPHeader header = BMPHeader(image);
    f.write(reinterpret_cast<char*>(header.first_header.magic), sizeof(header.first_header.magic));
    f.write(reinterpret_cast<char*>(header.first_header.size_bytes), sizeof(header.first_header.size_bytes));
    f.write(reinterpret_cast<char*>(header.first_header.zero_bytes), sizeof(header.first_header.zero_bytes));
    f.write(reinterpret_cast<char*>(header.second_header.size_byte), sizeof(header.second_header.size_byte));
    f.write(reinterpret_cast<char*>(header.second_header.zero_bytes), sizeof(header.second_header.zero_bytes));
    f.write(reinterpret_cast<char*>(header.second_header.width_bytes), sizeof(header.second_header.width_bytes));
    f.write(reinterpret_cast<char*>(header.second_header.height_bytes), sizeof(header.second_header.height_bytes));
    f.write(reinterpret_cast<char*>(header.second_header.first_byte_with_one),
            sizeof(header.second_header.first_byte_with_one));
    f.write(reinterpret_cast<char*>(header.second_header.first_zero_byte),
            sizeof(header.second_header.first_zero_byte));
    f.write(reinterpret_cast<char*>(header.second_header.byte_with_twenty_four),
            sizeof(header.second_header.byte_with_twenty_four));
    f.write(reinterpret_cast<char*>(header.second_header.last_zero_bytes),
            sizeof(header.second_header.last_zero_bytes));
    for (size_t i = 0; i < image.height_; ++i) {
        for (size_t j = 0; j < image.width_; ++j) {
            unsigned char red = static_cast<unsigned char>(GetPixel(image, j, i).red * MAX_COLOR_VALUE);
            unsigned char green = static_cast<unsigned char>(GetPixel(image, j, i).green * MAX_COLOR_VALUE);
            unsigned char blue = static_cast<unsigned char>(GetPixel(image, j, i).blue * MAX_COLOR_VALUE);
            unsigned char color[] = {blue, green, red};
            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(header.three_zeros), header.padding);
    }
}

void Read(Image& image, std::string path) {
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);
    BMPHeader header = BMPHeader();
    f.read(reinterpret_cast<char*>(header.first_header.magic), sizeof(header.first_header.magic));
    f.read(reinterpret_cast<char*>(header.first_header.size_bytes), sizeof(header.first_header.size_bytes));
    f.read(reinterpret_cast<char*>(header.first_header.zero_bytes), sizeof(header.first_header.zero_bytes));
    f.read(reinterpret_cast<char*>(header.second_header.size_byte), sizeof(header.second_header.size_byte));
    f.read(reinterpret_cast<char*>(header.second_header.zero_bytes), sizeof(header.second_header.zero_bytes));
    f.read(reinterpret_cast<char*>(header.second_header.width_bytes), sizeof(header.second_header.width_bytes));
    f.read(reinterpret_cast<char*>(header.second_header.height_bytes), sizeof(header.second_header.height_bytes));
    f.read(reinterpret_cast<char*>(header.second_header.first_byte_with_one),
           sizeof(header.second_header.first_byte_with_one));
    f.read(reinterpret_cast<char*>(header.second_header.first_zero_byte), sizeof(header.second_header.first_zero_byte));
    f.read(reinterpret_cast<char*>(header.second_header.byte_with_twenty_four),
           sizeof(header.second_header.byte_with_twenty_four));
    f.read(reinterpret_cast<char*>(header.second_header.last_zero_bytes), sizeof(header.second_header.last_zero_bytes));
    image.width_ = BytesMerge(header.second_header.width_bytes, 0);
    image.height_ = BytesMerge(header.second_header.height_bytes, 0);
    image.pixels_.resize(image.width_ * image.height_);
    const int remainder = static_cast<const int>((image.width_ * 3) % 4);
    const int padding = ((4 - remainder) % 4);
    for (size_t i = 0; i < image.height_; ++i) {
        for (size_t j = 0; j < image.width_; ++j) {
            unsigned char pixel[3];
            f.read(reinterpret_cast<char*>(pixel), 3);
            image.pixels_[i * image.width_ + j].red = static_cast<Color>(pixel[2]) / MAX_COLOR_VALUE;
            image.pixels_[i * image.width_ + j].green = static_cast<Color>(pixel[1]) / MAX_COLOR_VALUE;
            image.pixels_[i * image.width_ + j].blue = static_cast<Color>(pixel[0]) / MAX_COLOR_VALUE;
        }
        f.ignore(padding);
    }
}