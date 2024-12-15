#ifndef EXT4_FILESYSTEMS_HPP
#define EXT4_FILESYSTEMS_HPP

#include <ext4/filesystem_i.hpp>

#include <memory>
#include <filesystem>

namespace ext4
{

std::unique_ptr<filesystem_i> open(std::filesystem::path const & path);

}

#endif
