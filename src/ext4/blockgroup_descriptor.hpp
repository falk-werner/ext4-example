#ifndef EXT4_BLOCKGROUP_DESCRIPTOR_HPP
#define EXT4_BLOCKGROUP_DESCRIPTOR_HPP

#include <cinttypes>
#include <fstream>

namespace ext4
{

struct blockgroup_descriptor
{
    uint64_t block_bitmap;
    uint64_t inode_bitmap;
    uint64_t inode_table;

    void read(std::ifstream & stream, uint64_t offset, uint16_t size);
};

}

#endif
