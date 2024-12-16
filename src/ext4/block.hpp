#ifndef EXT4_BLOCK_HPP
#define EXP4_BLOCK_HPP

#include <cstddef>
#include <cinttypes>

namespace ext4
{

class block
{
    block(block const &) = delete;
    block& operator=(block const &) = delete;
public:
    block(size_t size_);
    ~block();
    block(block && other);
    block& operator=(block && other);
    uint8_t * data();
    size_t size() const;
private:
    uint8_t * m_data;
    size_t m_size;
};

}

#endif
