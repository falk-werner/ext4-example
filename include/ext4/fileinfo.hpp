#ifndef EXT4_FILEINFO_HPP
#define EXT4_FILEINFO_HPP

#include <ext4/filemode.hpp>

#include <ctime>
#include <cinttypes>

namespace ext4
{

struct fileinfo
{
    uint32_t inode_id;
    filemode mode;
    uint32_t uid;
    uint32_t gid;
    uint64_t size;
    timespec atime;
    timespec ctime;
    timespec mtime;
    uint16_t link_count;
    uint32_t generation;
};

}

#endif
