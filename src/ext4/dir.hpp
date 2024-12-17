#ifndef EXT4_DIR_HPP
#define EXT4_DIR_HPP

#include <ext4/dir_i.hpp>
#include <ext4/core_i.hpp>
#include <ext4/dir_entry.hpp>

#include <cinttypes>
#include <memory>

namespace ext4
{

class dir: public dir_i
{
public:
    dir(std::shared_ptr<core_i> core, uint32_t inode_id);
    ~dir() override = default;
    void scan(dir_entry_visitor visitor) override;
private:
    std::shared_ptr<core_i> m_core;
    uint32_t m_inode_id;
};

}

#endif
