#include "ext4/dir.hpp"
#include <stdexcept>

#include <iostream>

namespace ext4
{

dir::dir(std::shared_ptr<core_i> core, uint32_t inode_id)
: m_core(core)
{
    if (!m_core->lookup(inode_id, ino))
    {
        throw std::runtime_error("invalid dir inode");
    }

    std::cout << "mode : 0x" << std::hex << ino.mode << std::dec << std::endl;
    std::cout << "flags: 0x" << std::hex << ino.flags << std::dec << std::endl;
}

bool dir::get_next(dir_entry & out)
{
    return false;
}

}