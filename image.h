#include <string>
#include <vector>
#pragma once

using Color = float;

struct Pixel {
    Color red, green, blue;

    Pixel();

    Pixel(float red, float green, float blue);
};

struct Image {
    size_t width_, height_;
    std::vector<Pixel> pixels_;

    Image(size_t width, size_t height);
};

const Pixel& GetPixel(const Image& image, size_t i, size_t j);

void SetPixel(Image& image, const Pixel& pixel, size_t i, size_t j);

void BytesSplit(unsigned char* v, size_t x);

int BytesMerge(unsigned char* v, size_t index);

const int ZERO_BYTES_SIZE = 8;

struct FirstHeader {
    unsigned char magic[2] = {'B', 'M'};
    unsigned char size_bytes[4];
    unsigned char zero_bytes[ZERO_BYTES_SIZE];

    explicit FirstHeader();

    explicit FirstHeader(const int total_size);
};

const int LAST_ZERO_BYTES_SIZE = 25;
const int CHAR_BIT_SIZE = 8;

struct SecondHeader {
    unsigned char size_byte[1];
    unsigned char zero_bytes[3];
    unsigned char width_bytes[4];
    unsigned char height_bytes[4];
    unsigned char first_byte_with_one[1] = {1};
    unsigned char first_zero_byte[1] = {0};
    unsigned char byte_with_twenty_four[1] = {CHAR_BIT_SIZE * 3};
    unsigned char last_zero_bytes[LAST_ZERO_BYTES_SIZE];

    SecondHeader();

    SecondHeader(size_t width, size_t height, size_t second_header_size);
};

struct BMPHeader {
    unsigned char three_zeros[3] = {0, 0, 0};
    const int first_header_size = 14;
    const int second_header_size = 40;
    int remainder;
    int padding;
    int total_size;
    FirstHeader first_header;
    SecondHeader second_header;

    BMPHeader();

    explicit BMPHeader(Image image);
};

void Write(const Image& image, std::string path);

void Read(Image& image, std::string path);