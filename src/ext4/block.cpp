#include "ext4/block.hpp"

namespace ext4
{

block::block(size_t size_)
: m_data(new uint8_t[size_])
, m_size(size_)
{

}

block::~block()
{
    delete[] m_data;
}

block::block(block && other)
{
    if (this != &other)
    {
        this->m_data = other.m_data;
        other.m_data = nullptr;
        
        this->m_size = other.m_size;
        other.m_size = 0;
    }
}

block& block::operator=(block && other)
{
    if (this != &other)
    {
        this->m_data = other.m_data;
        other.m_data = nullptr;
        
        this->m_size = other.m_size;
        other.m_size = 0;
    }

    return *this;
}

uint8_t * block::data()
{
    return m_data;
}

size_t block::size() const
{
    return m_size;
}


}