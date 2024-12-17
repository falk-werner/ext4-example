#include "ext4/filesystem.hpp"
#include "ext4/dir.hpp"

#include <fstream>

namespace ext4
{

filesystem::filesystem(std::shared_ptr<core_i> core)
: m_core(core)
{

}

std::unique_ptr<dir_i> filesystem::get_root()
{
    auto root_dir = std::make_unique<dir>(m_core, 2);
    return std::move(root_dir);
}

bool filesystem::get_fileinfo(uint32_t inode_id, fileinfo & out)
{
    inode ino;
    if (!m_core->lookup(inode_id, ino))
    {
        return false;
    }

    out.inode_id = inode_id;
    out.mode = ino.mode;
    out.uid = ino.uid;
    out.gid = ino.gid;
    out.size = ino.size;

    // ToDo
    out.atime.tv_sec = 0;
    out.atime.tv_nsec = 0;
    out.ctime.tv_sec = 0;
    out.ctime.tv_nsec = 0;
    out.mtime.tv_sec = 0;
    out.mtime.tv_nsec = 0;
    out.link_count = 0;
    out.generation = 0;

    return true;
}

}