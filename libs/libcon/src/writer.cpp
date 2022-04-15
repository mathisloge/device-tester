#include "writer.hpp"
namespace dev::con
{
Writer::Writer()
    : tx_queue_{100}
{}
Writer::~Writer()
{}

void Writer::write(std::span<uint8_t> data)
{
    tx_queue_.push_front(std::vector<uint8_t>{data.begin(), data.end()});
    doWrite();
}

void Writer::write(std::vector<uint8_t> &&data)
{
    tx_queue_.push_front(std::forward<std::vector<uint8_t>>(data));
    doWrite();
}
} // namespace dev::con
