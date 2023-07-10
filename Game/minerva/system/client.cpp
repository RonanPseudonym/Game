#include "../include.h"

void Minerva::System::Client::OnInitialize(Engine* engine) {
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

	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);
	server.sin_addr.S_un.S_addr = inet_addr(server_ip);

	Send(new Net::Packet(
		Net::CLIENT_HANDSHAKE_REQUEST
	));
}

void Minerva::System::Client::Cycle1(Engine* engine) {
	while (packets.size()) {
		Net::Packet* p = packets.front();
		std::cout << p->ToString() << std::endl;
		packets.pop();
	}
}

void Minerva::System::Client::OnTerminate(Engine* engine) {
	Send(new Net::Packet(
		Net::CLIENT_DISCONNECT
	));
}

void Minerva::System::Client::Process1(Engine* engine, double delta) {
	SOCKADDR_IN from;
	int from_size = sizeof(from);
	int bytes_received = recvfrom(sock, buffer, 256, 0, (SOCKADDR*)&from, &from_size); // TODO: check if from is server

	if (bytes_received == SOCKET_ERROR)
	{
		Debug::Console::Error(("Recvfrom failed: " + std::to_string(WSAGetLastError())).c_str());
		return;
	}

	packets.push(new Net::Packet((unsigned char*)&buffer));
}

void Minerva::System::Client::Send(Net::Packet* packet) {
	Send((char*)packet->Dump(), packet->size);
}

void Minerva::System::Client::Send(char* data, unsigned int size) {
	int flags = 0;
	if (sendto(sock, data, size, flags, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		Debug::Console::Error(("Sendto failed: " + std::to_string(WSAGetLastError())).c_str());
		return;
	}

	free(data);
}