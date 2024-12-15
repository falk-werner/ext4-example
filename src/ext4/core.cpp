#include "ext4/core.hpp"

namespace ext4
{

core::core(std::filesystem::path const & path)
: stream(path)
{
    sb.parse(stream);
}


bool core::lookup(uint32_t inode_id, inode & out)
{
    if ((inode_id == 0) || (inode_id > sb.total_inodes))
    {
        throw std::runtime_error("invalid inode");
    }

    uint32_t const blockgroup_id = (inode_id - 1) / sb.inodes_per_group;
    blockgroup_descriptor group;
    get_blockgroup(blockgroup_id, group);

    uint64_t const inode_table_offset = group.inode_table * sb.block_size;
    uint32_t const inode_index = (inode_id - 1) % sb.inodes_per_group;
    uint64_t const inode_offset = inode_table_offset + (inode_index * sb.inode_size);

    out.read(stream, inode_offset, sb.inode_size);
    return true;
}

void core::get_blockgroup(uint32_t blockgroup_id, blockgroup_descriptor & out)
{
    uint32_t const bg_count = (sb.total_blocks + sb.blocks_per_group - 1) / sb.blocks_per_group;
    if (blockgroup_id > bg_count)
    {
        throw std::runtime_error("invalid blockgroup id");
    }

    uint64_t const offset = sb.gd_offset + (blockgroup_id * sb.bg_descriptor_size);


    out.read(stream, offset, sb.bg_descriptor_size);
}

}