#ifndef EXT4_FILEMODE_HPP
#define EXT4_FILEMODE_HPP

#include <cinttypes>

namespace ext4
{

constexpr uint16_t const mode_other_execute    = 0x0001;
constexpr uint16_t const mode_other_write      = 0x0002;
constexpr uint16_t const mode_other_read       = 0x0004;
constexpr uint16_t const mode_group_execute    = 0x0008;
constexpr uint16_t const mode_group_write      = 0x0010;
constexpr uint16_t const mode_group_read       = 0x0020;
constexpr uint16_t const mode_user_execute     = 0x0040;
constexpr uint16_t const mode_user_write       = 0x0080;
constexpr uint16_t const mode_user_read        = 0x0100;
constexpr uint16_t const mode_sticky           = 0x0200;
constexpr uint16_t const mode_suid             = 0x0400;
constexpr uint16_t const mode_sgid             = 0x0800;
constexpr uint16_t const mode_type_mask        = 0xf000;
constexpr uint16_t const mode_fifo             = 0x1000;
constexpr uint16_t const mode_character_device = 0x2000;
constexpr uint16_t const mode_directory        = 0x4000;
constexpr uint16_t const mode_block_device     = 0x6000;
constexpr uint16_t const mode_regular_file     = 0x8000;
constexpr uint16_t const mode_symbolic_link    = 0xA000;
constexpr uint16_t const mode_unix_socket      = 0xC000;

class filemode
{
public:
    filemode();
    filemode(uint16_t value);
    uint16_t value() const;
    bool other_read() const;
    bool other_write() const;
    bool other_execute() const;
    bool group_read() const;
    bool group_write() const;
    bool group_execute() const;
    bool user_read() const;
    bool user_write() const;
    bool user_execute() const;
    bool sticky() const;
    bool suid() const;
    bool sgid() const;
    bool is_fifo() const;
    bool is_character_device() const;
    bool is_directory() const;
    bool is_block_device() const;
    bool is_regular_file() const;
    bool is_symlink() const;
    bool is_socket() const;
private:
    uint16_t m_value;
};

}

#endif
