#ifndef EXT4_INODE_HPP
#define EXT4_INODE_HPP

#include <cinttypes>
#include <cstddef>
#include <fstream>

namespace ext4
{

constexpr size_t const ino_direct_blockpointers_size = 12;

struct inode
{
    uint16_t mode;
    uint32_t uid;
    uint32_t gid;
    uint32_t size;
    uint32_t flags;
    uint32_t direct_blockpointers[ino_direct_blockpointers_size];
    uint32_t singly_indirect_blockpointers;
    uint32_t doubly_indirect_blockpointers;
    uint32_t triply_indirect_blockpointers;

    void read(std::ifstream & stream, uint64_t offset, uint16_t size);
};

}

#endif
