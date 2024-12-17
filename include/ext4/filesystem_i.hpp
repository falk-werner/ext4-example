#ifndef EXT4_FILESYSTEM_I_HPP
#define EXT4_FILESYSTEM_I_HPP

#include <ext4/dir_entry.hpp>
#include <ext4/fileinfo.hpp>
#include <ext4/fsinfo.hpp>

#include <memory>
#include <functional>

namespace ext4
{

constexpr uint32_t const inode_id_root_directory = 2;

using dir_entry_visitor = std::function<bool (dir_entry const & entry)>;

class filesystem_i
{
public:
    virtual ~filesystem_i() = default;
    virtual void get_info(fsinfo & out) = 0;
    virtual void scan_directory(uint32_t inode_id, dir_entry_visitor visitor) = 0;
    virtual bool get_fileinfo(uint32_t inode_id, fileinfo & out) = 0;
};

}

#endif
