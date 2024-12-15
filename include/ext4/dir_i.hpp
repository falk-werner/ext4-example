#ifndef EXT4_DIR_I_HPP
#define EXT4_DIR_I_HPP

#include <ext4/dir_entry.hpp>

namespace ext4
{

class dir_i
{
public:
    virtual ~dir_i() = default;
    virtual bool get_next(dir_entry & entry) = 0;
};

}

#endif
