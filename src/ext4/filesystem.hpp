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
    std::unique_ptr<dir_i> get_root() override;
private:
    std::shared_ptr<core_i> m_core;
};

}

#endif
