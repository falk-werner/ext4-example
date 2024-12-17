#ifndef EXT4_DIR_I_HPP
#define EXT4_DIR_I_HPP

#include <ext4/dir_entry.hpp>
#include <functional>

namespace ext4
{

using dir_entry_visitor = std::function<bool (dir_entry const & entry)>;

class dir_i
{
public:
    virtual ~dir_i() = default;
    virtual void scan(dir_entry_visitor visitor) = 0;
};

}

#endif
