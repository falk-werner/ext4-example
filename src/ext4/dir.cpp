#include "ext4/dir.hpp"
#include "ext4/bytearray_reader.hpp"

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

    m_core->foreach_block(ino, [](uint8_t const * block, size_t size){
        bytearray_reader reader(block, size);
        size_t offset = 0;

        while (offset < size)
        {
            uint32_t inode_id = reader.u32(offset + 0);
            uint16_t record_size = reader.u16(offset + 4);

            if (inode_id != 0)
            {
                uint8_t name_length = block[offset + 6];
                uint8_t type = block[offset + 7];
                std::string name = reader.str(offset + 8, name_length);

                std::cout << name << std::endl;
            }

            offset += record_size;
        }

        return true;
    });

}

bool dir::get_next(dir_entry & out)
{
    return false;
}

}