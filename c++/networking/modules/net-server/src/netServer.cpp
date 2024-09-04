#include "net_connection.h"
#include "protoMessage.pb.h"
#include <cstdint>
#include <iostream>
#include <Olc_Net.h>

enum class CustomMsgTypes : uint32_t
{
    ServerAccept, // Fixed typo
    ServerDeny,
    ServerPing,
    ServerMessage,
    MessageAll,
    Echo,
};

class CustomServer : public Olc::Net::Server_Interface<CustomMsgTypes>
{
public:
    CustomServer(uint16_t nPort) :
        Olc::Net::Server_Interface<CustomMsgTypes>(nPort)
    {
    }

protected:
    virtual bool OnClientConnect(std::shared_ptr<Olc::Net::Connection<CustomMsgTypes>> client) override
    {
        std::cout << "\nOnClientConnect";
        Olc::Net::Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerAccept;
        client->Send(msg);
        return true;
    }

    // Called when a client appears to have disconnected
    virtual void OnClientDisconnect(std::shared_ptr<Olc::Net::Connection<CustomMsgTypes>> client) override
    {
        std::cout << "\nRemoving client [" << client->GetID() << "]";
    }

    // Called when a Message arrives
    virtual void OnMessage(std::shared_ptr<Olc::Net::Connection<CustomMsgTypes>> clientConnection, Olc::Net::Message<CustomMsgTypes> &msg) override
    {
        std::cout << "\nGet message";
        switch (msg.header.id)
        {
        case CustomMsgTypes::ServerPing: {
            std::cout << "\n[" << clientConnection->GetID() << "]: Server Ping";

            // Simply bounce Message back to client Echo
            clientConnection->Send(msg);
        }
        break;

        case CustomMsgTypes::MessageAll: {
            std::cout << "\n[" << clientConnection->GetID() << "]: Message All";

            // Construct a new Message and send it to all clients
            Olc::Net::Message<CustomMsgTypes> msg;
            msg.header.id = CustomMsgTypes::ServerMessage;
            msg << clientConnection->GetID();
            MessageAllClients(msg, clientConnection); // ignore self
        }
        break;
        case CustomMsgTypes::ServerAccept:
        case CustomMsgTypes::ServerDeny:
        case CustomMsgTypes::ServerMessage:
            break;
        case CustomMsgTypes::Echo: {
            // extract string
            std::string stringCode;
            msg >> stringCode;
            std::cout << "\n-----------------CODE----------------------------- ";
            std::cout << stringCode;
            std::cout << "\n-------------------End of Code----------------------";
            // deserialize
            ProtoMessage::Message protoFormat;
            protoFormat.ParseFromString(stringCode);
            std::cout << "\nGOT protoMessage: " << protoFormat.msg() << " " << protoFormat.val() << std::endl;
            // send it back
            Olc::Net::Message<CustomMsgTypes> serverMsg;
            serverMsg.header.id = CustomMsgTypes::Echo;
            serverMsg << stringCode;
            MessageClient(clientConnection, serverMsg);
        }
        break;
        }
    }
};

int main()
{
    CustomServer server(60000);
    server.Start();

    // std::string seed = "123";
    // std::string res  = Olc::Net::Connection<std::string>::Scramble(seed);
    // std::cout<<"\n"<<res;
    // printf("Digest is: ");
    // for (int i = 0; i < res.length(); i++)
    //     printf("%02x", static_cast<unsigned char>(res[i]));

    while (1)
    {
        server.Update(1, true);
    }

    return 0;
}