#pragma once

#include "net_common.h"

namespace Olc
{

    namespace Net
    {

        // use "enum class" to ensure the messages are valid at compile time
        template <typename T>
        struct Message_header
        {
        public:
            T id{};
            uint32_t size = 0; // message size
        };

        template <typename T>
        struct Message
        {
        public:
            /// @brief get the size of message
            /// @return size
            size_t size()
            {
                return sizeof(Message_header<T>) + body.size();
            }

            /// @brief print header info
            /// @param os
            /// @param message
            /// @return
            friend std::ostream &operator<<(std::ostream &os, const Message<T> &message)
            {
                os << "id: " << (int)message.header.id << " size " << message.size();
                return os;
            }

            /// @brief  push data
            /// @tparam T2
            /// @param msg
            /// @param data
            /// @return
            template <typename T2>
            friend Message<T> &operator<<(Message<T> &msg, const T2 &data)
            {
                static_assert(std::is_standard_layout<T2>::value, "Date is too complex and can not be copied");

                size_t msgBodySize = msg.body.size();
                // extend the body
                msg.body.resize(msg.body.size() + sizeof(T2));
                // copy data to msg body
                std::memcpy(msg.body.data() + msgBodySize, &data, sizeof(T2));
                // update size
                msg.header.size = msg.size();

                return msg;
            }

            /// @brief  extract data
            /// @tparam T2
            /// @param msg
            /// @param data
            /// @return
            template <typename T2>
            friend Message<T> &operator>>(Message<T> &msg, T2 &outData)
            {
                static_assert(std::is_standard_layout<T2>::value, "Date is too complex and can not be extract");
                size_t startingIndex = msg.body.size() - sizeof(T2);
                std::memcpy(&outData, msg.body.data() + startingIndex, sizeof(T2));
                msg.body.reserve(startingIndex);
                msg.header.size = msg.size();
                return msg;
            }

        public:
            /* data */
            Message_header<T> header{};
            std::vector<uint8_t> body;
        };

    }
}