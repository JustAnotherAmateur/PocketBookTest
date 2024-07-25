#pragma once

#include "Codec_global.h"

#include "RawImageData.h"

namespace codec
{
CODEC_EXPORT RawImageData::PixelContainer encode(const RawImageData& input_image, const std::size_t block_width = 4);
CODEC_EXPORT RawImageData decode(const RawImageData::PixelContainer& input_data, const std::size_t block_width = 4);
}
