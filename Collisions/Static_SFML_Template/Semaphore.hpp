#pragma once

#include <mutex>
#include <condition_variable>

namespace gen {

    class Semaphore {

    public:

        Semaphore(unsigned long count = 0) {
            m_count = count;
            }

        void signal() {
            std::unique_lock<decltype(m_mutex)> lock(m_mutex);
            m_count += 1;
            m_condition.notify_one();
            }

        void wait() {
            std::unique_lock<decltype(m_mutex)> lock(m_mutex);
            while (!m_count)
                m_condition.wait(lock);
            m_count -= 1;
            }

        bool try_wait() {
            std::unique_lock<decltype(m_mutex)> lock(m_mutex);
            if (m_count) {
                m_count -= 1;
                return true;
                }
            return false;
            }

        private:

            std::mutex m_mutex;
            std::condition_variable m_condition;
            unsigned m_count;

        };

    }