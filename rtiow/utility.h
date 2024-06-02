#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

#include <fstream>
#include <vector>
#include <cmath>
#include <string>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;



const double inf = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BMPInfoHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

inline std::string convertPPMtoBMP(const std::string& ppmFile, const std::string& bmpFile) {
    std::ifstream ppm(ppmFile);
    if (!ppm) {
        std::cerr << "Error opening PPM file." << std::endl;
        return "";
    }

    std::string header;
    int width, height, maxColorValue;
    ppm >> header >> width >> height >> maxColorValue;
    ppm.ignore();

    if (header != "P3") {
        std::cerr << "Invalid PPM file." << std::endl;
        return "";
    }

    std::vector<uint8_t> pixels(width * height * 3);
    for (int i = 0; i < width * height * 3; ++i) {
        int value;
        ppm >> value;
        pixels[i] = static_cast<uint8_t>(value);
    }

    BMPHeader bmpHeader;
    BMPInfoHeader bmpInfoHeader;

    int rowSize = (width * 3 + 3) & ~3;
    int dataSize = rowSize * height;

    bmpHeader.bfType = 0x4D42;
    bmpHeader.bfSize = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + dataSize;
    bmpHeader.bfReserved1 = 0;
    bmpHeader.bfReserved2 = 0;
    bmpHeader.bfOffBits = sizeof(BMPHeader) + sizeof(BMPInfoHeader);

    bmpInfoHeader.biSize = sizeof(BMPInfoHeader);
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = -height; // Negative to indicate top-down bitmap
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24;
    bmpInfoHeader.biCompression = 0;
    bmpInfoHeader.biSizeImage = dataSize;
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    std::ofstream bmp(bmpFile, std::ios::binary);
    if (!bmp) {
        std::cerr << "Error opening BMP file." << std::endl;
        return "";
    }

    bmp.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(bmpHeader));
    bmp.write(reinterpret_cast<const char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));

    std::vector<uint8_t> row(rowSize);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            size_t ppmIndex = (y * width + x) * 3;
            size_t bmpIndex = x * 3;
            row[bmpIndex] = pixels[ppmIndex + 2];
            row[bmpIndex + 1] = pixels[ppmIndex + 1];
            row[bmpIndex + 2] = pixels[ppmIndex];
        }
        bmp.write(reinterpret_cast<const char*>(row.data()), rowSize);
    }

    std::string bmpFileName = ppmFile[0] == '.' ? "output.bmp" : ppmFile + ".bmp";
    return bmpFileName;
}

inline void openBMPFile(const std::string& bmpFile) {
    // std::string command = "open " + bmpFile; // macOS
    std::string command = "start " + bmpFile; // Windows
    // std::string command = "xdg-open " + bmpFile; // Linux
    system(command.c_str());
}

#include "color.h"
#include "ray.h"
#include "vec3.h"

#endif // !UTILITY_H
