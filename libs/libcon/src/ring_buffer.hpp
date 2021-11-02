#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <boost/circular_buffer.hpp>

template <class T>
class RingBuffer
{
  public:
    typedef boost::circular_buffer<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;

    explicit RingBuffer(size_type capacity)
        : m_unread(0)
        , m_container(capacity)
    {}
    RingBuffer(const RingBuffer &) = delete;
    RingBuffer &operator=(const RingBuffer &) = delete;

    void push_front(T &&item)
    {
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            m_not_full.wait(lock, std::bind(&RingBuffer<value_type>::is_not_full, this));
            m_container.push_front(std::forward<T>(item));
            ++m_unread;
        }
        m_not_empty.notify_one();
    }

    T pop_back()
    {
        T item;
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            m_not_empty.wait(lock, std::bind(&RingBuffer<value_type>::is_not_empty, this));
            item = m_container[--m_unread];
        }
        m_not_full.notify_one();
        return item;
    }

    bool is_not_empty() const
    {
        return m_unread > 0;
    }
    bool is_not_full() const
    {
        return m_unread < m_container.capacity();
    }

  private:
    size_type m_unread;
    container_type m_container;
    std::mutex m_mutex;
    std::condition_variable m_not_empty;
    std::condition_variable m_not_full;
};
