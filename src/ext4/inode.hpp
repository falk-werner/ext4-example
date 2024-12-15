#ifndef EXT4_INODE_HPP
#define EXT4_INODE_HPP

#include <cinttypes>
#include <cstddef>
#include <fstream>

namespace ext4
{

struct inode
{
    uint16_t mode;
    uint32_t uid;
    uint32_t gid;
    uint32_t size;
    uint32_t flags;

    void read(std::ifstream & stream, uint64_t offset, uint16_t size);
};

}

#endif
