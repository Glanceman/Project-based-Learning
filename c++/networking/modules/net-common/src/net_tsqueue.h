#pragma once

#include <mutex>
#include <deque>
#include "net_common.h"

namespace Olc
{

    namespace Net
    {
        template <typename T>
        class Tsqueue
        {
        public:
            Tsqueue() = default;
            Tsqueue(const Tsqueue<T> &) = delete; // copy constructor
            virtual ~Tsqueue() { clear(); }
            /// @brief Returns items at the front of the queue
            /// @return
            const T &front()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.front();
            }

            /// @brief return item at the back of the queue
            /// @return
            const T &back()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.back();
            }

            /// @brief add item at the back of the queue
            /// @param item
            void emplace_back(const T &item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.emplace_back(std::move(item));
            }

            /// @brief add item at the front of the queue
            /// @param item
            void emplace_front(const T &item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.emplace_front(std::move(item));
            }

            /// @brief return true if it is empty
            /// @return
            bool empty()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.empty();
            }

            size_t count()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.count();
            }

            void clear()
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.clear()
            }

            T pop_front()
            {
                std::scoped_lock lock(muxQueue);
                T temp = std::move(deqQueue.front());
                deqQueue.pop_front();
                return temp;
            }

            T pop_back()
            {
                std::scoped_lock lock(muxQueue);
                T temp = std::move(deqQueue.back());
                deqQueue.pop_back();
                return temp;
            }

        protected:
            std::mutex mtx;
            std::deque<T> deqQueue;
        };

    }
}