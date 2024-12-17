#include "ext4/filesystem.hpp"
#include "ext4/bytearray_reader.hpp"

#include <fstream>

namespace ext4
{

filesystem::filesystem(std::shared_ptr<core_i> core)
: m_core(core)
{

}

void filesystem::get_info(fsinfo & out)
{
    m_core->get_info(out);
}


void filesystem::scan_directory(uint32_t inode_id, dir_entry_visitor visitor)
{
    inode ino;
    if (!m_core->lookup(inode_id, ino))
    {
        throw std::runtime_error("invalid dir inode");
    }

    filemode mode(ino.mode);
    if (!mode.is_directory())
    {
        throw std::runtime_error("inode doest not refer to a directory");
    }

    m_core->foreach_block(ino, [&visitor](uint8_t const * block, size_t size){
        bool continue_ = true;
        bytearray_reader reader(block, size);
        size_t offset = 0;

        while (offset < size)
        {
            uint32_t inode_id = reader.u32(offset + 0);
            uint16_t record_size = reader.u16(offset + 4);

            if (inode_id != 0)
            {
                uint8_t name_length = block[offset + 6];

                dir_entry entry;
                entry.inode = inode_id;
                entry.type = file_type::unknown; // block[offset + 7];
                entry.name = reader.str(offset + 8, name_length);

                continue_ = visitor(entry);
            }

            offset += record_size;
        }

        return continue_;
    });
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