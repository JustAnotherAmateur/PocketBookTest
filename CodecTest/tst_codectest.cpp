#include <QtTest>

#include "../Codec/codec.h"

using namespace codec;

namespace
{
bool operator==(const RawImageData& img1, const RawImageData& img2)
{
    return (img1.m_height == img2.m_height) && (img1.m_width == img2.m_width) &&
           (img1.m_pixel_data == img2.m_pixel_data);
}

RawImageData::PixelContainer create_vector(std::initializer_list<RawImageData::PixelType> init_list)
{
    return RawImageData::PixelContainer(init_list);
}
}

class CodecTest : public QObject
{
    Q_OBJECT

public:
    CodecTest() = default;
    ~CodecTest() = default;

private slots:
    void test_data();

    void test();
};

void CodecTest::test_data()
{
    QTest::addColumn<RawImageData>("image");
    QTest::addColumn<RawImageData::PixelContainer>("encoded");

    QTest::newRow("basic_1") << RawImageData(4, 2,
                                             {
                                                 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF
                                             })
                             << create_vector({'B', 'A', 0, 4, 0, 2,
                                               1, 1});

    QTest::newRow("basic_2") << RawImageData(4, 2,
                                             {
                                                 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0x00, 0x00, 0x00, 0x00
                                             })
                             << create_vector({'B', 'A', 0, 4, 0, 2,
                                               1, 0,
                                               0b10});

    QTest::newRow("basic_3") << RawImageData(4, 2,
                                             {
                                                 0xF1, 0xFF, 0xFF, 0xFF,
                                                 0x00, 0x00, 0x00, 0x00
                                             })
                             << create_vector({'B', 'A', 0, 4, 0, 2,
                                               0, 0,
                                               0b11, 0xF1, 0xFF, 0xFF, 0xFF,
                                               0b10});

    QTest::newRow("complex") << RawImageData(8, 4,
                                             {
                                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x06, 0x06,
                                                 0x07, 0x07, 0x08, 0x08, 0xFF, 0xFF, 0xFF, 0xFF
                                             })
                             << create_vector({'B', 'A', 0, 8, 0, 4,
                                               1, 0, 0, 0,
                                               0b10, 0b10,
                                               0b10, 0b11, 0x05, 0x05, 0x06, 0x06,
                                               0b11, 0x07, 0x07, 0x08, 0x08, 0b00});

    QTest::newRow("irregular_size_1") << RawImageData(1, 1,
                                                      {
                                                          0xFF
                                                      })
                                      << create_vector({'B', 'A', 0, 1, 0, 1,
                                                        1});

    QTest::newRow("irregular_size_2") << RawImageData(1, 1,
                                                      {
                                                          0x00
                                                      })
                                      << create_vector({'B', 'A', 0, 1, 0, 1,
                                                        0,
                                                        0b10});

    QTest::newRow("irregular_size_3") << RawImageData(6, 2,
                                                      {
                                                          0x05, 0x05, 0x00, 0x00, 0x00, 0x00,
                                                          0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x07
                                                      })
                                      << create_vector({'B', 'A', 0, 6, 0, 2,
                                                        0, 0,
                                                        0b11, 0x05, 0x05, 0x00, 0x00, 0b10,
                                                        0b00, 0b11, 0x07, 0x07});
}

void CodecTest::test()
{
    QFETCH(RawImageData, image);
    QFETCH(RawImageData::PixelContainer, encoded);

    QCOMPARE(encode(image), encoded);
    QCOMPARE(decode(encoded), image);
}

QTEST_APPLESS_MAIN(CodecTest)

#include "tst_codectest.moc"
