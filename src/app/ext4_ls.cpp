#include "ext4/ext4.hpp"

#include <string>
#include <iostream>

namespace 
{

char ls_type(ext4::filemode mode)
{
    if (mode.sticky())  { return 'S'; }
    if (mode.is_fifo()) { return 'p'; }
    if (mode.is_character_device()) { return 'c'; }
    if (mode.is_directory()) { return 'd'; }
    if (mode.is_block_device()) { return 'b'; }
    if (mode.is_symlink()) { return 'l'; }
    if (mode.is_socket()) { return 's'; }

    return '-';
}

char ls_ur(ext4::filemode mode)
{
    return mode.user_read() ? 'r' : '-';
}

char ls_uw(ext4::filemode mode)
{
    return mode.user_write() ? 'w' : '-';
}

char ls_ux(ext4::filemode mode)
{
    if (mode.suid()) { return 's'; }
    return mode.user_execute() ? 'x' : '-';
}

char ls_gr(ext4::filemode mode)
{
    return mode.group_read() ? 'r' : '-';
}

char ls_gw(ext4::filemode mode)
{
    return mode.group_write() ? 'w' : '-';
}

char ls_gx(ext4::filemode mode)
{
    if (mode.sgid()) { return 's'; }
    return mode.group_execute() ? 'x' : '-';
}

char ls_or(ext4::filemode mode)
{
    return mode.other_read() ? 'r' : '-';
}

char ls_ow(ext4::filemode mode)
{
    return mode.other_write() ? 'w' : '-';
}

char ls_ox(ext4::filemode mode)
{
    return mode.other_execute() ? 'x' : '-';
}


}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string const filename = argv[1];
        auto fs = ext4::open(filename);

        auto root_dir = fs->get_root();
        root_dir->scan([&fs](auto const & entry) {
            ext4::fileinfo info;
            if (!fs->get_fileinfo(entry.inode, info)) {
                std::cerr << "failed to lookup inode: " << entry.name << ": " << entry.inode << std::endl;                
            }

            std::cout <<
                std::setw(10) << info.inode_id << ' ' << 
                ls_type(info.mode) <<
                ls_ur(info.mode) <<
                ls_uw(info.mode) <<
                ls_ux(info.mode) <<
                ls_gr(info.mode) <<
                ls_gw(info.mode) <<
                ls_gx(info.mode) <<
                ls_or(info.mode) <<
                ls_ow(info.mode) <<
                ls_ox(info.mode) <<
                ' ' <<
                info.link_count << ' ' << 
                info.uid << ' ' <<
                info.gid << ' ' <<
                std::setw(10) << info.size << ' ' <<
                entry.name <<
                std::endl;

            return true;
        });
    }
    else
    {
        std::cout << "usage: ext4-ls <filename>" << std::endl;
        return 1;
    }

    return 0;
}