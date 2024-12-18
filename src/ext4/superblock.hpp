#ifndef EXT4_SUPERBLOCK_HPP
#define EXT4_SUPERBLOCK_HPP

#include <cinttypes>
#include <cstddef>

#include <fstream>
#include <string>

namespace ext4
{

struct superblock
{
    // basic fields
    uint32_t total_inodes;
    uint64_t total_blocks;
    uint64_t reserved_blocks;
    uint64_t free_blocks;
    uint32_t free_inodes;
    uint32_t first_data_block;
    uint64_t block_size;
    uint32_t blocks_per_group;
    uint32_t inodes_per_group;
    uint16_t state;
    uint16_t errors;
    uint16_t minor_revision;
    uint32_t creator_os;
    uint32_t revision;
    
    // extended fields
    uint32_t first_ino;
    uint16_t inode_size;
    uint32_t feature_compat;
    uint32_t feature_incompat;
    uint32_t feature_ro_compat;
    std::string uuid;
    std::string volume_name;

    uint64_t gd_offset;
    uint16_t bg_descriptor_size;

    void parse(std::ifstream & stream);
};


}

#endif
