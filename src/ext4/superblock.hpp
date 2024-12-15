#ifndef EXT4_SUPERBLOCK_HPP
#define EXT4_SUPERBLOCK_HPP

#include <cinttypes>
#include <cstddef>
#include <fstream>

namespace ext4
{

struct superblock
{
    uint32_t total_inodes;
    uint64_t total_blocks;
    uint64_t block_size;
    uint32_t blocks_per_group;
    uint32_t inodes_per_group;
    uint16_t inode_size;
    uint64_t gd_offset;
    uint16_t bg_descriptor_size;

    void parse(std::ifstream & stream);
};


}

#endif
