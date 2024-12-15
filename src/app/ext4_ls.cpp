#include "ext4/ext4.hpp"

#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string const filename = argv[1];
        auto fs = ext4::open(filename);

        auto root_dir = fs->get_root();
        ext4::dir_entry entry;
        while (root_dir->get_next(entry))
        {
            std::cout << entry.name << std::endl;
        }
    }
    else
    {
        std::cout << "usage: ext4-ls <filename>" << std::endl;
        return 1;
    }

    return 0;
}