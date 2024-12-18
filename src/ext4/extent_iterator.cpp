#include "ext4/extent_iterator.hpp"

#include <iostream>
#include <stdexcept>

namespace
{

constexpr size_t const extent_header_magic_offset = 0;
constexpr size_t const extent_header_entries_offset = 2;
constexpr size_t const extent_header_max_offset = 4;
constexpr size_t const extent_header_depth_offset = 6;
constexpr size_t const extent_header_generation_offset = 8;

constexpr size_t const extent_block_offset = 0;
constexpr size_t const extent_len_offset = 4;
constexpr size_t const extent_start_hi_offset = 6;
constexpr size_t const extent_start_lo_offset = 8;

constexpr uint16_t const extent_header_magic = 0xf30a;
constexpr uint16_t const extent_header_max_depth = 5;

constexpr uint16_t const extent_header_size = 12;
constexpr uint16_t const extent_entry_size = 12;
constexpr uint16_t const extent_len_max = 32768;


struct extent_header
{
    uint16_t magic;
    uint16_t entries;
    uint16_t max;
    uint16_t depth;
    uint32_t generation;
};

void read_header(ext4::bytearray_reader & reader, extent_header & header)
{
    header.magic = reader.u16(extent_header_magic_offset);
    header.entries = reader.u16(extent_header_entries_offset);
    header.max = reader.u16(extent_header_max_offset);
    header.depth = reader.u16(extent_header_depth_offset);
    header.generation = reader.u32(extent_header_generation_offset);

    if (header.magic != extent_header_magic)
    {
        throw std::runtime_error("invalid extent header magic");
    }

    if (header.depth > extent_header_max_depth)
    {
        throw std::runtime_error("extent max depth (5) exceeded");
    }
}

struct extent_intermediate
{
    uint32_t file_block;
    uint64_t child;
};

struct extent
{
    uint32_t file_block;
    uint16_t length;
    uint64_t start;
};

void read_extent(ext4::bytearray_reader & reader, size_t n, extent & out)
{
    size_t offset = extent_header_size + (extent_entry_size * n);
    out.file_block = reader.u32(offset + extent_block_offset);
    out.length = reader.u16(offset + extent_len_offset);
    out.start = reader.u32(offset + extent_start_lo_offset);
    out.start += static_cast<uint64_t>(reader.u16(offset + extent_start_hi_offset)) << 32;

    // ignore uninitialized blocks
    if (out.length > extent_len_max)
    {
        out.length = 0;
    }
}

}

namespace ext4
{

extent_iterator::extent_iterator(core_i & core)
: m_core(core)
{

}

void extent_iterator::foreach_block(std::string const & block, block_visitor visitor)
{
    bytearray_reader reader(reinterpret_cast<uint8_t const*>(block.data()), block.size());

    extent_header header;
    read_header(reader, header);
    if (header.depth == 0)
    {
        process_blocks(reader, header.entries, visitor);
    }
    else
    {
        process_intermediate(reader, header.entries, header.depth, visitor);
    }
}

bool extent_iterator::visit_block(uint64_t block_id, block_visitor & visitor)
{
    if (block_id == 0) { return true; }

    auto block = m_core.read_block(block_id);
    if (!block.has_value())
    {
        std::cerr << "error: core: failed to read block #" << block_id << std::endl;
        return false;
    }

    try
    {            
        return visitor(block.value().data(), block.value().size());
    }
    catch (...)
    {
        std::cerr << "error: core: block visitor throws" << std::endl;
        return false;
    }
}

bool extent_iterator::process_blocks(bytearray_reader & reader, uint16_t entries, block_visitor & visitor)
{
    bool continue_ = true;

    for(size_t i = 0; i < entries; i++)
    {
        extent ext;
        read_extent(reader, i, ext);

        if (ext.start > 0)
        {
            for(uint64_t j = 0; continue_ && (j < ext.length); j++)
            {
                uint64_t const block_id = ext.start + j;
                continue_ = visit_block(block_id, visitor);
            }
        }
    }

    return continue_;
}

bool extent_iterator::process_intermediate(bytearray_reader & reader, uint16_t entries, uint16_t depth, block_visitor & visitor)
{
    // ToDo: implement me
    return false;
}


}