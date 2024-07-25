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
    void test_encode();
};

void CodecTest::test_encode()
{
    QCOMPARE(encode(RawImageData(4, 2,
                                {
                                    0xFF, 0xFF, 0xFF, 0xFF,
                                    0xFF, 0xFF, 0xFF, 0xFF
                                })),
             create_vector({4, 2, 1, 1}));

}

QTEST_APPLESS_MAIN(CodecTest)

#include "tst_codectest.moc"
