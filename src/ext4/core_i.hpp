#ifndef EXT4_CORE_I_HPP
#define EXT4_CORE_I_HPP

#include "ext4/inode.hpp"
#include "ext4/fsinfo.hpp"
#include "ext4/block.hpp"

#include <cinttypes>
#include <functional>
#include <optional>

namespace ext4
{

using block_visitor = std::function<bool (uint8_t const * block, size_t blocksize)>;

class core_i
{
public:
    virtual ~core_i() = default;
    virtual void get_info(fsinfo & out) = 0;
    virtual bool lookup(uint32_t inode_id, inode & out) = 0;
    virtual void foreach_block(inode const & inode, block_visitor visitor) = 0;
    virtual std::optional<block> read_block(uint64_t block_id) = 0;

};

}

#endif
