#include <iostream>
#include <olc_net.h>
#include <chrono>
#include <Windows.h> // For GetAsyncKeyState

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,  // Fixed typo
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
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

    bool key[3] = {false, false, false};
    bool old_key[3] = {false, false, false};

    bool bQuit = false;
    while (!bQuit)
    {
        // Get user input
        key[0] = GetAsyncKeyState('1') & 0x8000;
        key[1] = GetAsyncKeyState('2') & 0x8000;
        key[2] = GetAsyncKeyState('3') & 0x8000;

        // Take action
        if (key[0] && !old_key[0])
            c.PingServer();
        if (key[1] && !old_key[1])
            c.MessageAll();
        if (key[2] && !old_key[2])
            bQuit = true;

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
                case CustomMsgTypes::ServerMessage:
                    std::cout << "\nServer message received.";
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