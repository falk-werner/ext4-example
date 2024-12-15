#include <ext4/crc32.hpp>
#include <gtest/gtest.h>

TEST(crc32, zero)
{
    ext4::crc32 checksum;
    ASSERT_EQ(0, checksum.get_checksum());
}

TEST(crc32, one)
{
    ext4::crc32 checksum;
    checksum.update(static_cast<uint8_t>('1'));
    ASSERT_EQ(0, checksum.get_checksum());
}