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

}