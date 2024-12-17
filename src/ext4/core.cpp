#include "ext4/core.hpp"
#include "ext4/bytearray_reader.hpp"

#include <iostream>

namespace ext4
{

core::core(std::filesystem::path const & path)
: stream(path)
{
    sb.parse(stream);
    m_block = std::make_unique<block>(sb.block_size);
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
    foreach_direct_block(inode, visitor) &&
    foreach_indirect_block(inode.singly_indirect_blockpointers, visitor) &&
    foreach_doubly_indirect_block(inode.doubly_indirect_blockpointers, visitor) &&
    foreach_triply_indirect_block(inode.triply_indirect_blockpointers, visitor);

}

bool core::read_block(uint64_t block_id)
{
    uint64_t const offset = block_id * sb.block_size;
    stream.seekg(offset);
    if (!stream.good())
    {
        return false;
    }

    stream.read(reinterpret_cast<char*>(m_block->data()), m_block->size());
    return stream.good();
}

bool core::visit_block(uint64_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    if (!read_block(block_id))
    {
        std::cerr << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    try
    {            
        return visitor(m_block->data(), m_block->size());
    }
    catch (...)
    {
        std::cerr << "error: core: block visitor throws" << std::endl;
        return false;
    }
}


bool core::foreach_direct_block(inode const & inode, block_visitor & visitor)
{
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < ino_direct_blockpointers_size); i++)
    {
        uint64_t const block_id = static_cast<uint64_t>(inode.direct_blockpointers[i]);
        continue_ = visit_block(block_id, visitor);
    }

    return continue_;
}

bool core::foreach_indirect_block(uint32_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    if (!read_block(static_cast<uint64_t>(block_id)))
    {
        std::cout << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    constexpr size_t const address_size = 4;
    size_t const count = sb.block_size / address_size;
    bytearray_reader reader(m_block->data(), m_block->size());
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < count); i++)
    {
        uint32_t const address = reader.u32(i * address_size);
        continue_ = visit_block(address, visitor);
    }

    return continue_;
}

bool core::foreach_doubly_indirect_block(uint32_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    if (!read_block(static_cast<uint64_t>(block_id)))
    {
        std::cout << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    constexpr size_t const address_size = 4;
    size_t const count = sb.block_size / address_size;
    bytearray_reader reader(m_block->data(), m_block->size());
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < count); i++)
    {
        uint32_t const address = reader.u32(i * address_size);
        continue_ = foreach_indirect_block(address, visitor);
    }

    return continue_;
}

bool core::foreach_triply_indirect_block(uint32_t block_id, block_visitor & visitor)
{    
    if (block_id == 0) { return true; }

    if (!read_block(static_cast<uint64_t>(block_id)))
    {
        std::cout << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    constexpr size_t const address_size = 4;
    size_t const count = sb.block_size / address_size;
    bytearray_reader reader(m_block->data(), m_block->size());
    bool continue_ = true;
    for(size_t i = 0; continue_ && (i < count); i++)
    {
        uint32_t const address = reader.u32(i * address_size);
        continue_ = foreach_doubly_indirect_block(address, visitor);
    }
    return continue_;
}


}