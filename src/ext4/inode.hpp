#ifndef EXT4_INODE_HPP
#define EXT4_INODE_HPP

#include <cinttypes>
#include <cstddef>
#include <fstream>

namespace ext4
{

constexpr uint32_t const ino_flag_extents     = 0x00080000;
constexpr uint32_t const ino_flag_inline_data = 0x10000000;

struct inode
{
    uint16_t mode;
    uint32_t uid;
    uint32_t gid;
    uint32_t size;
    uint32_t flags;

    std::string block;

    void read(std::ifstream & stream, uint64_t offset, uint16_t ino_size);
};

}

#endif
