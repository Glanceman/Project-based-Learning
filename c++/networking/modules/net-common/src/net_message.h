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
            T        id;       // message type
            uint32_t size = 0; // message size including header
        };

        template <typename T>
        struct Message
        {
        public:
            /// @brief get the size of message content
            /// @return size
            size_t size()
            {
                return body.size();
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
                static_assert(std::is_standard_layout<T2>::value, "Data is too complex and can not be copied");

                size_t msgBodySize = msg.body.size();
                // extend the body
                msg.body.resize(msg.body.size() + sizeof(T2));
                // copy data to msg body
                std::memcpy(msg.body.data() + msgBodySize, &data, sizeof(T2));
                // update size
                msg.header.size = msg.size();

                return msg;
            }

            /// @brief Push std::string into the message (overload for std::string)
            /// @param msg Reference to the Message object
            /// @param data Const reference to the string to be pushed
            /// @return Reference to the modified Message object
            friend Message<T> &operator<<(Message<T> &msg, const std::string &data)
            {
                size_t msgBodySize = msg.body.size();
                size_t strLength   = data.length();
                // todo use reserve
                //  Extend the body to accommodate string data and its length
                msg.body.resize(msgBodySize + strLength + sizeof(size_t));

                // Copy string data
                std::memcpy(msg.body.data() + msgBodySize, data.c_str(), strLength);

                // Append string length
                std::memcpy(msg.body.data() + msgBodySize + strLength, &strLength, sizeof(size_t));

                // Update size
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
                static_assert(std::is_standard_layout<T2>::value, "Data is too complex and can not be extract");

                size_t startingIndex = msg.body.size() - sizeof(T2);

                std::memcpy(&outData, msg.body.data() + startingIndex, sizeof(T2));

                msg.body.resize(startingIndex);

                msg.header.size = msg.size();
                return msg;
            }

            /// @brief Extract std::string from the message (overload for std::string)
            /// @param msg Reference to the Message object
            /// @param outData Reference to the string where extracted data will be stored
            /// @return Reference to the modified Message object
            friend Message<T> &operator>>(Message<T> &msg, std::string &outData)
            {
                size_t strLength;
                size_t startingIndex = msg.body.size() - sizeof(size_t);

                // Extract the string length
                std::memcpy(&strLength, msg.body.data() + startingIndex, sizeof(size_t));

                // Resize the output string and copy the data
                outData.resize(strLength);
                std::memcpy(&outData[0], msg.body.data() + startingIndex - strLength, strLength);

                // Resize the body to remove the extracted string and its length
                msg.body.resize(startingIndex - strLength);

                msg.header.size = msg.size();
                return msg;
            }

        public:
            /* data */
            Message_header<T>    header{};
            std::vector<uint8_t> body;
        };

        template <typename T>
        class Connection;

        template <typename T>
        struct OwnedMessage
        {
            std::shared_ptr<Connection<T>> remoteConnection = nullptr;

            Message<T> msg;

            friend std::ostream &operator<<(std::ostream &os, const OwnedMessage<T> &message)
            {
                os << message.msg;
                return os;
            }
        };

        template <typename T, typename T2>
        class CustomMessage
        {
        };
    } // namespace Net
} // namespace Olc