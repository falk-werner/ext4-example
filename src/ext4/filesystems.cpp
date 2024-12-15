#include "ext4/filesystems.hpp"
#include "ext4/filesystem.hpp"
#include "ext4/core.hpp"

namespace ext4
{

std::unique_ptr<filesystem_i> open(std::filesystem::path const & path)
{
    auto core = std::make_shared<ext4::core>(path);
    auto fs = std::make_unique<filesystem>(std::move(core));
    return std::move(fs);
}

}