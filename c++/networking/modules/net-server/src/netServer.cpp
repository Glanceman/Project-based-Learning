#include <iostream>
#include <Olc_Net.h>

enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};



class CustomServer : public Olc::Net::Server_Interface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort) : Olc::Net::Server_Interface<CustomMsgTypes>(nPort)
	{

	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<Olc::Net::Connection<CustomMsgTypes>> client)
	{
		Olc::Net::Message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<Olc::Net::Connection<CustomMsgTypes>> client)
	{
		// std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	// Called when a Message arrives
	virtual void OnMessage(std::shared_ptr<Olc::Net::Connection<CustomMsgTypes>> client, Olc::Net::Message<CustomMsgTypes>& msg)
	{
		// switch (msg.header.id)
		// {
		// case CustomMsgTypes::ServerPing:
		// {
		// 	std::cout << "[" << client->GetID() << "]: Server Ping\n";

		// 	// Simply bounce Message back to client
		// 	client->Send(msg);
		// }
		// break;

		// case CustomMsgTypes::MessageAll:
		// {
		// 	std::cout << "[" << client->GetID() << "]: Message All\n";

		// 	// Construct a new Message and send it to all clients
		// 	Olc::Net::Message<CustomMsgTypes> msg;
		// 	msg.header.id = CustomMsgTypes::ServerMessage;
		// 	msg << client->GetID();
		// 	MessageAllClients(msg, client);

		// }
		// break;
		// }
	}
};

int main()
{
	CustomServer server(60000); 
	server.Start();

	while (1)
	{
		server.Update(-1);
	}
	


	return 0;
}