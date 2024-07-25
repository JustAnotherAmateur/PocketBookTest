#include "codec.h"

#include <limits>

namespace codec
{
using PixelType = RawImageData::PixelType;

const PixelType WHITE_PIXEL = std::numeric_limits<PixelType>::max();
const PixelType BLACK_PIXEL = 0;

const PixelType WHITE_BLOCK_MARKER = 0b00;
const PixelType BLACK_BLOCK_MARKER = 0b10;
const PixelType MIXED_BLOCK_MARKER = 0b11;

RawImageData::PixelContainer encode(const RawImageData& input_image, const std::size_t block_width)
{
    RawImageData::PixelContainer coded_data;
    coded_data.emplace_back(static_cast<PixelType>('B'));
    coded_data.emplace_back(static_cast<PixelType>('A'));

    // TODO: handle cases when sizeof(width) > sizeof(PixelType)
    coded_data.emplace_back(static_cast<PixelType>(input_image.m_width));
    coded_data.emplace_back(static_cast<PixelType>(input_image.m_height));

    const auto white_rows_first_index = coded_data.size();
    for (auto i = 0; i < input_image.m_height; ++i)
    {
        coded_data.emplace_back(false);
    }

    for (auto row = 0; row < input_image.m_height; ++row)
    {
        const auto row_start_index = row * input_image.m_width;

        bool is_row_white = true;
        for (auto col = 0; (col < input_image.m_width) && is_row_white; ++col)
        {
            if (input_image.m_pixel_data[row_start_index + col] != WHITE_PIXEL)
            {
                is_row_white = false;
            }
        }

        if (is_row_white)
        {
            coded_data[white_rows_first_index + row] = true;
        }
        else
        {
            for (auto col = 0; col < input_image.m_width; col += block_width)
            {
                bool is_block_white = true;
                bool is_block_black = true;
                for (auto i = 0; (i < block_width) && (col + i < input_image.m_width); ++i)
                {
                    is_block_white &= (input_image.m_pixel_data[row_start_index+col+i] == WHITE_PIXEL);
                    is_block_black &= (input_image.m_pixel_data[row_start_index+col+i] == BLACK_PIXEL);
                }

                if (is_block_white)
                {
                    coded_data.emplace_back(WHITE_BLOCK_MARKER);
                }
                else if (is_block_black)
                {
                    coded_data.emplace_back(BLACK_BLOCK_MARKER);
                }
                else
                {
                    coded_data.emplace_back(MIXED_BLOCK_MARKER);
                    for (auto i = 0; (i < block_width) && (col + i < input_image.m_width); ++i)
                    {
                        coded_data.emplace_back(input_image.m_pixel_data[row_start_index+col+i]);
                    }
                }
            } // for (auto col
        } // else (is_row_white
    } // for (auto row

    return coded_data;
}

RawImageData decode(const RawImageData::PixelContainer& input_data, const std::size_t block_width)
{
    RawImageData decoded_image{};

    std::size_t index = 0;
    // TODO: check BA
    ++index;
    ++index;

    decoded_image.m_width = input_data[index++];
    decoded_image.m_height = input_data[index++];
    decoded_image.m_pixel_data.reserve(decoded_image.m_width * decoded_image.m_height);

    std::vector<bool> is_row_white(decoded_image.m_height);
    for (auto i = 0; i < is_row_white.size(); ++i, ++index)
    {
        is_row_white[i] = static_cast<bool>(input_data[index]);
    }

    for (auto row = 0; row < decoded_image.m_height; ++row)
    {
        if (is_row_white[row])
        {
            for (auto i = 0; i < decoded_image.m_width; ++i)
            {
                decoded_image.m_pixel_data.emplace_back(WHITE_PIXEL);
            }
        }
        else
        {
            std::size_t col = 0;

            switch (input_data[index++])
            {
            case WHITE_BLOCK_MARKER:
                {
                for (auto i = 0; (i < block_width) && (col < decoded_image.m_width); ++i, ++col)
                {
                    decoded_image.m_pixel_data.emplace_back(WHITE_PIXEL);
                }
                }
            break;

            case BLACK_BLOCK_MARKER:
                {
                    for (auto i = 0; (i < block_width) && (col < decoded_image.m_width); ++i, ++col)
                    {
                        decoded_image.m_pixel_data.emplace_back(BLACK_PIXEL);
                    }
                }
            break;

            case MIXED_BLOCK_MARKER:
                {
                    for (auto i = 0; (i < block_width) && (col < decoded_image.m_width); ++i, ++col, ++index)
                    {
                        decoded_image.m_pixel_data.emplace_back(input_data[index]);
                    }
                }
            break;

            // default: throw
            } // switch
        } // else is_row_white
    } // for (auto row

    return decoded_image;
}
} // namespace
