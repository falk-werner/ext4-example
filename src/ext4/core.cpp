#include "ext4/core.hpp"
#include "ext4/bytearray_reader.hpp"
#include "ext4/block_iterator.hpp"
#include "ext4/extent_iterator.hpp"

#include <iostream>

namespace ext4
{

core::core(std::filesystem::path const & path)
: stream(path)
{
    sb.parse(stream);
}

void core::get_info(fsinfo & out)
{
    out.total_inodes = sb.total_inodes;
    out.total_blocks = sb.total_blocks;
    out.reserved_blocks = sb.reserved_blocks;
    out.free_blocks = sb.free_blocks;
    out.free_inodes = sb.free_inodes;
    out.first_data_block = sb.first_data_block;
    out.block_size = sb.block_size;
    out.blocks_per_group = sb.blocks_per_group;
    out.inodes_per_group = sb.inodes_per_group;
    out.state = sb.state;
    out.errors = sb.errors;
    out.minor_revision = sb.minor_revision;
    out.creator_os = sb.creator_os;
    out.revision = sb.revision;
    out.first_ino = sb.first_ino;
    out.inode_size = sb.inode_size;
    out.feature_compatible = sb.feature_compat;
    out.feature_incompatible = sb.feature_incompat;
    out.feature_ro_compatible = sb.feature_ro_compat;
    out.blockgroup_descriptor_size = sb.bg_descriptor_size;
    out.uuid = sb.uuid;
    out.volume_name = sb.volume_name;
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

void core::foreach_block(inode const & inode, block_visitor visitor)
{
    if ((inode.flags & ino_flag_inline_data) != 0)
    {
        visitor(reinterpret_cast<uint8_t const *>(inode.block.data()), inode.block.size());
    }
    else if ((inode.flags & ino_flag_extents) != 0)
    {
        extent_iterator it(*this);
        it.foreach_block(inode.block, visitor);
    }
    else
    {
        block_iterator it(*this);
        it.foreach_block(inode, visitor);
    }
}

std::optional<block> core::read_block(uint64_t block_id)
{
    uint64_t const offset = block_id * sb.block_size;
    stream.seekg(offset);
    if (!stream.good())
    {
        return std::nullopt;
    }

    block block(sb.block_size);
    stream.read(reinterpret_cast<char*>(block.data()), block.size());
    if (!stream.good())
    {
        return std::nullopt;
    }

    return std::move(block);
}



}