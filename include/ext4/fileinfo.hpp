#ifndef EXT4_FILEINFO_HPP
#define EXT4_FILEINFO_HPP

#include <ctime>
#include <cinttypes>

namespace ext4
{

struct fileinfo
{
    uint16_t mode;
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
