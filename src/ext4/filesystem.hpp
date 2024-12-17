#ifndef EXTFS_FILESYSTEM_HPP
#define EXTFS_FILESYSTEM_HPP

#include "ext4/filesystem_i.hpp"
#include "ext4/core_i.hpp"

#include <filesystem>

namespace ext4
{

class filesystem: public filesystem_i
{
public:
    filesystem(std::shared_ptr<core_i> core);
    ~filesystem() override = default;
    void get_info(fsinfo & out) override;
    void scan_directory(uint32_t inode_id, dir_entry_visitor visitor) override;
    bool get_fileinfo(uint32_t inode_id, fileinfo & out) override;

private:
    std::shared_ptr<core_i> m_core;
};

}

#endif
