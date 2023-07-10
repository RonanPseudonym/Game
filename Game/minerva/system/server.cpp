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
		
		std::cout << p->data.ToString() << std::endl;

		packets.pop();
	}
}

void Minerva::System::Server::Cycle2(Engine* engine) {
	// TODO: send world state
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
	int flags = 0;
	SOCKADDR_IN from;
	int from_size = sizeof(from);
	int bytes_received = recvfrom(sock, buffer, 256, flags, (SOCKADDR*)&from, &from_size);

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