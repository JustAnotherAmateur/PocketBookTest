#include "codecthread.h"

#include "../Codec/codec.h"

#include <QImage>
#include <QFile>

#include <algorithm>
#include <stdexcept>

namespace
{
codec::RawImageData read_image(const QString& filename)
{
    QImage image(filename);
    codec::RawImageData image_data;
    image_data.m_width = image.width();
    image_data.m_height = image.height();
    image_data.m_pixel_data.reserve(image_data.m_width * image_data.m_height);

    if ((image_data.m_width == 0) || (image_data.m_height == 0))
        throw std::runtime_error("Empty image");

    for (auto y = 0; y < image.height(); ++y)
    {
        for (auto x = 0; x < image.width(); ++x)
        {
            image_data.m_pixel_data.emplace_back(qGray(image.pixel(x, y)));
        }
    }
    return image_data;
}

void write_image(const codec::RawImageData& image_data, const QString& filename)
{
    QFile file(filename);
    if (file.exists())
        throw std::runtime_error("File already exists");

    const auto bytes_per_line = image_data.m_width;
    QImage image(image_data.m_pixel_data.data(), image_data.m_width, image_data.m_height, bytes_per_line, QImage::Format_Grayscale8);
    if (!image.save(filename))
        throw std::runtime_error("Failed to save image");
}

codec::RawImageData::PixelContainer read_encoded_data(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        throw std::runtime_error("Failed to open file");
    const auto bytes = file.readAll();
    file.close();

    codec::RawImageData::PixelContainer encoded_data(bytes.size());
    std::copy(bytes.cbegin(), bytes.cend(), encoded_data.begin());

    return encoded_data;
}

void write_encoded_data(const codec::RawImageData::PixelContainer& data, const QString& filename)
{
    QByteArray bytes(data.size(), Qt::Uninitialized);
    std::copy(data.cbegin(), data.cend(), bytes.begin());

    QFile file(filename);
    if (!file.open(QFile::NewOnly))
        throw std::runtime_error("File already exists");
    file.write(bytes);
    file.close();
}

void encode_file(const QString& input_filename, const QString& out_filename)
{
    const auto image = read_image(input_filename);
    const auto encoded = codec::encode(image);
    write_encoded_data(encoded, out_filename);
}

void decode_file(const QString& input_filename, const QString& out_filename)
{
    const auto encoded = read_encoded_data(input_filename);
    const auto image = codec::decode(encoded);
    write_image(image, out_filename);
}
} // namespace


CodecThread::CodecThread(QObject* parent, const QString &source_filepath, const QString &target_filepath, const EMode mode)
    : QThread(parent)
    , m_source_filepath(source_filepath)
    , m_target_filepath(target_filepath)
    , m_mode(mode)
{
}

void CodecThread::run()
{
    try
    {
        if (m_mode == EMode::Encode)
        {
            encode_file(m_source_filepath, m_target_filepath);
        }
        else if (m_mode == EMode::Decode)
        {
            decode_file(m_source_filepath, m_target_filepath);
        }
    }
    catch (const std::runtime_error& e)
    {
        emit error(QString::fromUtf8(e.what()));
    }
}
