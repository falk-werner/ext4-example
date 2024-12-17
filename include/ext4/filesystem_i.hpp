#ifndef EXT4_FILESYSTEM_I_HPP
#define EXT4_FILESYSTEM_I_HPP

#include <ext4/dir_i.hpp>
#include <ext4/fileinfo.hpp>

#include <memory>

namespace ext4
{

class filesystem_i
{
public:
    virtual ~filesystem_i() = default;
    virtual std::unique_ptr<dir_i> get_root() = 0;
    virtual bool get_fileinfo(uint32_t inode_id, fileinfo & out) = 0;
};

}

#endif
