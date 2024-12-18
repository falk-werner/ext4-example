#ifndef EXT4_BLOCKITERATOR_HPP
#define EXT4_BLOCKITERATOR_HPP

#include "ext4/core_i.hpp"
#include "ext4/block.hpp"

namespace ext4
{

class block_iterator
{
public:
    block_iterator(core_i & core);
    void foreach_block(inode const & inode, block_visitor visitor);
private:
    bool visit_block(uint64_t block_id, block_visitor & visitor);
    bool foreach_direct_block(inode const & inode, block_visitor & visitor);
    bool foreach_indirect_block(uint32_t block_id, block_visitor & visitor);
    bool foreach_doubly_indirect_block(uint32_t block_id, block_visitor & visitor);
    bool foreach_triply_indirect_block(uint32_t block_id, block_visitor & visitor);

    core_i & m_core;
    block m_block;
};

}

#endif