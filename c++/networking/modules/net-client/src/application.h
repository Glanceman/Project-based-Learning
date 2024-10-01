#include "imgui.h"
#include "appConsole.h"
#include "protoMessage.pb.h"

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
        msg.header.id                                 = CustomMsgTypes::ServerPing;
        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
        msg << timeNow;
        Send(msg);
    }

    void EchoServer()
    {
        std::cout << "\nEchoAll";
        ProtoMessage::Message protoFormat;
        protoFormat.set_msg("Hello World");
        protoFormat.set_val(689);
        std::string stringCode = protoFormat.SerializeAsString();

        Olc::Net::Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::Echo;
        msg << stringCode;

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

/*

Application

*/
class Application
{
public:
    // copy constructor
    Application(Application &)                  = delete;
    Application &operator=(const Application &) = delete;
    // move constructor
    Application(Application &&)            = delete;
    Application &operator=(Application &&) = delete;

    void Init();

    void BeginPlay();

    void Update();

    static Application &GetInstance()
    {
        static Application instance;
        return instance;
    }

private:
    Application();
    ~Application();

public:
    bool    show_demo_window        = false;
    bool    show_another_window     = false;
    bool    show_application_window = true;
    ImVec4  clear_color             = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGuiIO io;

private:
    AppConsole   console;
    bool         show_application_console = true;
    bool         prevConnectStatus        = false;
    CustomClient client;
};