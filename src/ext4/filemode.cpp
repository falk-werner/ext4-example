#include "ext4/filemode.hpp"

namespace ext4
{

filemode::filemode()
: m_value(0)
{
}

filemode::filemode(uint16_t value)
: m_value(value)
{

}

uint16_t filemode::value() const
{
    return m_value;
}

bool filemode::other_read() const
{
    return (m_value & mode_other_read) != 0;
}

bool filemode::other_write() const
{
    return (m_value & mode_other_write) != 0;
}

bool filemode::other_execute() const
{
    return (m_value & mode_other_execute) != 0;
}

bool filemode::group_read() const
{
    return (m_value & mode_group_read) != 0;
}

bool filemode::group_write() const
{
    return (m_value & mode_group_write) != 0;
}

bool filemode::group_execute() const
{
    return (m_value & mode_group_execute) != 0;
}

bool filemode::user_read() const
{
    return (m_value & mode_user_read) != 0;
}

bool filemode::user_write() const
{
    return (m_value & mode_user_write) != 0;
}

bool filemode::user_execute() const
{
    return (m_value & mode_user_execute) != 0;
}

bool filemode::sticky() const
{
    return (m_value & mode_sticky) != 0;
}

bool filemode::suid() const
{
    return (m_value & mode_suid) != 0;
}

bool filemode::sgid() const
{
    return (m_value & mode_sgid) != 0;
}

bool filemode::is_fifo() const
{
    return (m_value & mode_type_mask) == mode_fifo;
}

bool filemode::is_character_device() const
{
    return (m_value & mode_type_mask) == mode_character_device;
}

bool filemode::is_directory() const
{
    return (m_value & mode_type_mask) == mode_directory;
}

bool filemode::is_block_device() const
{
    return (m_value & mode_type_mask) == mode_block_device;
}

bool filemode::is_regular_file() const
{
    return (m_value & mode_type_mask) == mode_regular_file;
}

bool filemode::is_symlink() const
{
    return (m_value & mode_type_mask) == mode_symbolic_link;
}

bool filemode::is_socket() const
{
    return (m_value & mode_type_mask) == mode_unix_socket;
}



}