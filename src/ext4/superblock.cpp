#include "superblock.hpp"
#include "ext4/bytearray_reader.hpp"
#include <stdexcept>

namespace ext4
{

namespace
{
    constexpr const size_t superblock_offset = 1024;

    constexpr uint16_t ext2_signature = 0xef53;

    constexpr uint32_t sb_feature_64bit = 0x80;

    constexpr const size_t sb_total_inodes_offset = 0;
    constexpr const size_t sb_total_blocks_lo_offset = 4;
    constexpr const size_t sb_total_blocks_hi_offset = 0x150;
    constexpr const size_t sb_first_data_block_offset = 20;
    constexpr const size_t sb_block_size_offset = 24;
    constexpr const size_t sb_blocks_per_group_offset = 32;
    constexpr const size_t sb_inodes_per_group_offset = 40;
    constexpr const size_t sb_ext2_signature_offset = 56;
    constexpr const size_t sb_filesystem_state_offset = 58;
    constexpr const size_t sb_major_version_offset = 76;
    constexpr const size_t sb_first_inode_offset = 84;
    constexpr const size_t sb_inode_size_offset = 88;
    constexpr const size_t sb_required_features_offset = 96;
    constexpr const size_t sb_checksum_offset = 1020;
    constexpr const size_t sb_bg_descr_size_offset = 0xFE;
    constexpr const size_t sb_incompat_features_offset = 0x60;

    constexpr const size_t superblock_size = 1024;
}

void superblock::parse(std::ifstream & stream)
{
    uint8_t data[superblock_size];
    auto reader = bytearray_reader::from_stream(stream, data, superblock_offset, superblock_size);

    uint16_t const signature = reader.u16(sb_ext2_signature_offset);
    if (signature != ext2_signature)
    {
        throw std::runtime_error("invalid signature");
    }

    uint32_t const major_version = reader.u32(sb_major_version_offset);

    total_blocks = reader.u32(sb_total_blocks_lo_offset);
    total_inodes = reader.u32(sb_total_inodes_offset);
    blocks_per_group = reader.u32(sb_blocks_per_group_offset);
    inodes_per_group = reader.u32(sb_inodes_per_group_offset);
    inode_size = 128;
    bg_descriptor_size = 32;

    uint64_t const first_data_block = static_cast<uint64_t>(reader.u32(sb_first_data_block_offset));

    uint32_t const ld_block_size = reader.u32(sb_block_size_offset);
    if (ld_block_size > 6)
    {
        throw std::runtime_error("block size above 64KByte is not supported");
    }
    block_size = 1 << (10 + ld_block_size);


    if (major_version >= 1)
    {
        uint64_t const total_blocks_high = static_cast<uint64_t>(reader.u32(sb_total_blocks_hi_offset));
        total_blocks += total_blocks_high << 32;

        inode_size = reader.u16(sb_inode_size_offset);

        uint32_t const incompat_features = reader.u32(sb_incompat_features_offset);
        if ((incompat_features & sb_feature_64bit) != 0)
        {
            bg_descriptor_size = reader.u16(sb_bg_descr_size_offset);
        }
    }

    gd_offset = (first_data_block + 1) * block_size;
}


// uint64_t superblock::get_block_group_of_inode(uint32_t inode) const
// {
//     if ((inode == 0) || (inode > total_inodes))
//     {
//         throw std::runtime_error("invalid inode");
//     }
// 
//     return ((inode - 1) / inodes_per_group);
// }


}