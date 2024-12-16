#ifndef EXT4_CORE_HPP
#define EXT4_CORE_HPP

#include "ext4/core_i.hpp"
#include "ext4/superblock.hpp"
#include "ext4/blockgroup_descriptor.hpp"
#include "ext4/block.hpp"

#include <filesystem>
#include <fstream>

namespace ext4
{

class core: public core_i
{
public:
    core(std::filesystem::path const & path);
    ~core() override = default;
    bool lookup(uint32_t inode_id, inode & out) override;
    void foreach_block(inode const & inode, block_visitor visitor) override;

private:
    void get_blockgroup(uint32_t blockgroup_id, blockgroup_descriptor & out);

    bool read_block(uint64_t block_id);
    bool visit_block(uint64_t block_id, block_visitor & visitor);
    bool foreach_direct_block(inode const & inode, block_visitor & visitor);
    bool foreach_indirect_block(uint32_t block_id, block_visitor & visitor);
    bool foreach_doubly_indirect_block(uint32_t block_id, block_visitor & visitor);
    bool foreach_triply_indirect_block(uint32_t block_id, block_visitor & visitor);

    std::ifstream stream;
    superblock sb;
    std::unique_ptr<block> m_block;
};

}

#endif