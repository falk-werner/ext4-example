#ifndef EXT4_DIR_ENTRY_HPP
#define EXT4_DIR_ENTRY_HPP

#include <cinttypes>
#include <string>

namespace ext4
{

enum class file_type
{
    unknown = 0,
    regular_file = 1,
    directory = 2,
    character_device = 3,
    block_device = 4,
    fifo = 5,
    socket = 6,
    symbolic_link = 7
};

struct dir_entry
{
    uint32_t inode;
    std::string name;
    file_type type;
};

}

#endif
