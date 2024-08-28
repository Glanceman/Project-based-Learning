#include "protoMessage.pb.h"

#include <exception>
#include <iostream>
#include <olc_net.h>
#include <chrono>
#include <Windows.h> // For GetAsyncKeyState



enum class CustomMsgTypes : uint32_t
{
    ServerAccept,  
    ServerDeny,
    ServerPing,
    ServerMessage,
    MessageAll,
    Echo,
};

class CustomClient : public Olc::Net::Client_Interface<CustomMsgTypes>
{
public:
    void PingServer()
    {
        std::cout << "\nPing";
        Olc::Net::Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerPing;
        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
        msg << timeNow;
        Send(msg);
    }

    void EchoServer(){
        std::cout << "\nEchoAll";
        ProtoMessage::Message protoFormat;
        protoFormat.set_msg("Hello World");
        protoFormat.set_val(689);
        std::string stringCode = protoFormat.SerializeAsString();

        Olc::Net::Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::Echo;
        msg<<stringCode;
        
        Send(msg);
    }

    void MessageAll()
    {
        std::cout << "\nMessageAll";
        Olc::Net::Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::MessageAll;
        Send(msg);
    }
};

int main()
{
    CustomClient c;
    if (!c.Connect("127.0.0.1", 60000))
    {
        std::cerr << "Failed to connect to server!" << std::endl;
        return 1;
    }

    bool key[4] = {false, false, false,false};
    bool old_key[4] = {false, false, false,false};

    bool bQuit = false;
    while (!bQuit)
    {
        // Get user input
        key[0] = GetAsyncKeyState('1') & 0x8000;
        key[1] = GetAsyncKeyState('2') & 0x8000;
        key[2] = GetAsyncKeyState('3') & 0x8000;
        key[3] = GetAsyncKeyState('4') & 0x8000;

        // Take action
        if (key[0] && !old_key[0])
            c.EchoServer();
        if (key[1] && !old_key[1])
            c.MessageAll();
        if (key[2] && !old_key[2])
            bQuit = true;
        // if (key[3] && !old_key[3])
        //     c.EchoServer();


        // Record old key states
        for (int i = 0; i < 3; i++)
            old_key[i] = key[i];

        if (c.IsConnected())
        {
            while (!c.Incoming().empty())
            {
                auto msg = c.Incoming().pop_front().msg;

                switch (msg.header.id)
                {
                case CustomMsgTypes::ServerPing:
                {
                    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                    std::chrono::system_clock::time_point timeThen;
                    msg >> timeThen;
                    std::cout << "\nPing: " << std::chrono::duration<double>(timeNow - timeThen).count() << " seconds";
                    break;
                }
                case CustomMsgTypes::ServerAccept:
                    std::cout << "\nServer accepted the connection.";
                    break;
                case CustomMsgTypes::ServerDeny:
                    std::cout << "\nServer denied the connection.";
                    bQuit = true;
                    break;
                case CustomMsgTypes::ServerMessage:{
                        uint32_t clientID;
                        msg >> clientID;
                        std::cout << "\nHello from [" << clientID << "]";
                    }
                    break;
                case CustomMsgTypes::Echo:{
                    std::string stringCode;
                    try {
                        msg >> stringCode;
                        std::cout<<"\nCODE " << stringCode;

                        ProtoMessage::Message protoFormat;
                        protoFormat.ParseFromString(stringCode);
                        std::cout << "\nGOT protoMessage: " << protoFormat.msg() << " " << protoFormat.val();

                    } catch (std::exception e) {
                        std::cout<<"Error " << stringCode;
                    }
                }
                break;
                default:
                    std::cout << "\nUnknown message type received.";
                    break;
                }
            }
        }
        else
        {
            std::cout << "\nServer disconnected.";
            bQuit = true;
        }
    }

    std::cout << "\nClient shutting down...";
    return 0;
}