#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE

#include <iostream>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include "protoMessage.pb.h"

std::vector<char> vBuffer(1 * 1024);
void GrabSomeData(asio::ip::tcp::socket &socket)
{
    // write data into the buffer
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
                           [&](std::error_code ec, std::size_t length)
                           {
                               if (!ec)
                               {
                                   std::cout << "\n\nRead " << length << "bytes\n\n";
                                   for (int i = 0; i < length; i++)
                                   {
                                       std::cout << vBuffer[i];
                                   }
                                   GrabSomeData(socket);
                               }
                           });
}

int main(int, char **)
{
    std::cout << "APP Start";

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    try
    {
        ProtoMessage::Message msg;
        msg.set_msg("Hello World");
        msg.set_val(689);
        std::cout << "protoMessage: " << msg.msg() << " " << msg.val() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        google::protobuf::ShutdownProtobufLibrary();
        return 1;
    }

    google::protobuf::ShutdownProtobufLibrary();

    asio::error_code error_code;
    asio::io_context context; // the platform specific interface

    // allow the context to keep running
    asio::io_context::work idleWork(context);

    std::thread thrContext = std::thread([&]()
                                         { 
                                            /*allow async operation to be able to run without blocking the main thread*/
                                            context.run(); 
                                            std::cout << "\nContext finish"; });
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("172.217.27.4", error_code), 80); // create address
    asio::ip::tcp::socket socket(context);                                                    // open socket
    socket.connect(endpoint, error_code);

    if (error_code)
    {
        std::cout << "\nConnection failure" << error_code.message();
    }
    else
    {
        std::cout << "\nConnection success";
    }

    if (socket.is_open())
    {
        GrabSomeData(socket);

        std::string request =
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        // send
        socket.write_some(asio::buffer(request.data(), request.size()), error_code);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);

        context.stop();
        if (thrContext.joinable())
        {
            thrContext.join();
        }
    }
}
