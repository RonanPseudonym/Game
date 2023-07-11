#include "../include.h"

void Minerva::System::Server::OnInitialize(Engine* engine) {
	WORD winsock_version = 0x202;
	WSADATA winsock_data;
	if (WSAStartup(winsock_version, &winsock_data))
	{
		Debug::Console::Error(("WSAStartup failed: " + std::to_string(WSAGetLastError())).c_str());
		return;
	}

	int address_family = AF_INET;
	int type = SOCK_DGRAM;
	int protocol = IPPROTO_UDP;
	sock = socket(address_family, type, protocol);

	if (sock == INVALID_SOCKET)
	{
		Debug::Console::Error(("Socket failed: " + std::to_string(WSAGetLastError())).c_str());
		return;
	}

	SOCKADDR_IN local_address;
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(port);
	local_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
	{
		Debug::Console::Error(("Bind failed: " + std::to_string(WSAGetLastError())).c_str());
		return;
	}
}

void Minerva::System::Server::Cycle1(Engine* engine) {
	while (packets.size()) {
		Net::ReceivedPacket* p = packets.front();
		
		switch (p->data.type) {
			case Net::CLIENT_HANDSHAKE_REQUEST: {
				Net::Packet reply(Net::SERVER_HANDSHAKE_REPLY);
				if (prefs.max_players > 0 && clients.size() >= prefs.max_players) {
					reply += new Net::IntU2(0);
				}
				else {
					reply += new Net::IntU2(1);
					clients.push_back(p->from);
					Debug::Console::Log((std::string(inet_ntoa(p->from.sin_addr)) + " connected").c_str());
				}

				Send(&reply, (SOCKADDR*)&(p->from));
				break;
			}
			case Net::CLIENT_DISCONNECT: {
				Debug::Console::Log((std::string(inet_ntoa(p->from.sin_addr)) + " disconnected").c_str());
				
				for (int i = 0; i < clients.size(); i ++) {
					SOCKADDR_IN addr = clients[i];
					if ((addr.sin_addr.S_un.S_addr == p->from.sin_addr.S_un.S_addr) && (addr.sin_port == p->from.sin_port)) {
						clients.erase(clients.begin() + i);
						break;
					}
				}

				break;
			}
		}

		packets.pop();
	}
}

void Minerva::System::Server::Cycle2(Engine* engine) {
	Net::Packet state(Net::SERVER_SNAPSHOT);
	char* data = (char*)state.Dump();

	for (SOCKADDR_IN client : clients) { // TODO: broadcasting?
		Send(data, state.size, (SOCKADDR*)&client);
	}
}

void Minerva::System::Server::Send(char* data, unsigned int size, SOCKADDR* to) {
	if (sendto(sock, data, size, 0, to, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		Debug::Console::Error(("Sendto failed: " + std::to_string(WSAGetLastError())).c_str());
		return;
	}
}

void Minerva::System::Server::Send(Net::Packet* packet, SOCKADDR* to) {
	Send((char*)packet->Dump(), packet->size, to);
}

void Minerva::System::Server::OnTerminate(Engine* engine) {

}

void Minerva::System::Server::Process1(Engine* engine, double delta) {
	SOCKADDR_IN from;
	int from_size = sizeof(from);
	int bytes_received = recvfrom(sock, buffer, 256, 0, (SOCKADDR*)&from, &from_size);

	if (bytes_received == SOCKET_ERROR)
	{
		Debug::Console::Error(("Recvfrom failed: " + std::to_string(WSAGetLastError())).c_str());
		return;
	}

	packets.push(
		new Net::ReceivedPacket(
			Net::Packet((unsigned char*)&buffer),
			from
		)
	);
}