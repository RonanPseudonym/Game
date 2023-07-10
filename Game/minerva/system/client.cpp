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
}

void Minerva::System::Client::OnUpdate(Engine* engine) {

}

void Minerva::System::Client::OnTerminate(Engine* engine) {

}

void Minerva::System::Client::OnThread(Engine* engine, double delta) {
	std::string text;
	std::cout << "> ";
	std::getline(std::cin, text);

	Net::Packet p = Net::Packet(Net::TEST);
	p += new Net::String((char*)text.c_str());

	Send(&p);
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