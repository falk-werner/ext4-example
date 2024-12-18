#ifndef EXT4_EXTENT_ITERATOR_HPP
#define EXT4_EXTENT_ITERATOR_HPP

#include "ext4/core_i.hpp"
#include "ext4/block.hpp"
#include "ext4/bytearray_reader.hpp"

#include <string>

namespace ext4
{

class extent_iterator
{
public:
    extent_iterator(core_i & core);
    void foreach_block(std::string const & block, block_visitor visitor);
private:
    bool visit_block(uint64_t block_id, block_visitor & visitor);
    bool process_blocks(bytearray_reader & reader, uint16_t entries, block_visitor & visitor);
    bool process_intermediate(bytearray_reader & reader, uint16_t entries, uint16_t depth, block_visitor & visitor);
    core_i & m_core;
};

}

#endif
