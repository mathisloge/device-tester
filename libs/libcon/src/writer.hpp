#pragma once
#include <cstdint>
#include <span>
#include <vector>
#include "ring_buffer.hpp"
class Writer
{
  public:
    Writer();
    virtual ~Writer();

    void write(std::span<uint8_t> data);
    void write(std::vector<uint8_t> &&data);

  protected:
    virtual void doWrite() = 0;

  protected:
    RingBuffer<std::vector<uint8_t>> tx_queue_;
    std::vector<uint8_t> tx_current_;
};
