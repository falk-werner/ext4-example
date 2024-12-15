#ifndef EXT4_CORE_I_HPP
#define EXT4_CORE_I_HPP

#include "ext4/inode.hpp"

#include <cinttypes>

namespace ext4
{

class core_i
{
public:
    virtual ~core_i() = default;
    virtual bool lookup(uint32_t inode_id, inode & out) = 0;
};

}

#endif
