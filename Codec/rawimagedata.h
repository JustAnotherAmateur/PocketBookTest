#pragma once

#include "Codec_global.h"

#include <cstddef>
#include <vector>

namespace codec
{
struct CODEC_EXPORT RawImageData
{
    using PixelType = unsigned char;
    using PixelContainer = std::vector<PixelType>;

    std::size_t m_width = 0;
    std::size_t m_height = 0;
    PixelContainer m_pixel_data;

    RawImageData() = default;
    RawImageData(const std::size_t width, const std::size_t height, std::initializer_list<PixelType> pixel_data)
        : m_width(width)
        , m_height(height)
        , m_pixel_data(pixel_data)
    {}
    RawImageData(const RawImageData&) = default;
    RawImageData(RawImageData&&) = default;
    ~RawImageData() = default;
};
}
