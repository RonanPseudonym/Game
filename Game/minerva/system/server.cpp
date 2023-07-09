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

void Minerva::System::Server::OnUpdate(Engine* engine) {

}

void Minerva::System::Server::OnTerminate(Engine* engine) {

}

void Minerva::System::Server::OnThread(Engine* engine, double delta) {
}