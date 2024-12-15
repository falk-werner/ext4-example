#include <gtest/gtest.h>
#include <ctime>

TEST(time, is_64bit)
{
    ASSERT_EQ(sizeof(uint64_t), sizeof(time_t));
}