#ifndef EXT4_CORE_HPP
#define EXT4_CORE_HPP

#include "ext4/core_i.hpp"
#include "ext4/superblock.hpp"
#include "ext4/blockgroup_descriptor.hpp"

#include <filesystem>
#include <fstream>

namespace ext4
{

class core: public core_i
{
public:
    core(std::filesystem::path const & path);
    virtual ~core() override = default;
    virtual bool lookup(uint32_t inode_id, inode & out);
private:
    void get_blockgroup(uint32_t blockgroup_id, blockgroup_descriptor & out);
    std::ifstream stream;
    superblock sb;
};

}

#endif