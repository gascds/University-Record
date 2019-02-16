#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdio.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"Iphlpapi.lib")

using namespace std;
int main();
string store[10];int ip_size = 0;

void getIPAddress() {
	PIP_ADAPTER_INFO IpAdapterInfo = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
	unsigned long size = sizeof(IP_ADAPTER_INFO);
	unsigned long info = GetAdaptersInfo(IpAdapterInfo, &size);

	if (info == ERROR_BUFFER_OVERFLOW)
	{
		free(IpAdapterInfo);
		IpAdapterInfo = (PIP_ADAPTER_INFO)malloc(size);
		info = GetAdaptersInfo(IpAdapterInfo, &size);
	}

	PIP_ADAPTER_INFO HeadIpInfo = IpAdapterInfo;
	if (info == ERROR_SUCCESS)
	{
		while (IpAdapterInfo)
		{
			PIP_ADDR_STRING IpAddress_string = &(IpAdapterInfo->IpAddressList);
			while (IpAddress_string)
			{
				store[ip_size] = IpAddress_string->IpAddress.String;
				++ip_size;
				IpAddress_string = IpAddress_string->Next;
			}
			IpAdapterInfo = IpAdapterInfo->Next;
		}
	}
	if (HeadIpInfo)
	{
		free(HeadIpInfo);
		HeadIpInfo = NULL;
	}
}



void cilent(string ipAddress, int port)
{
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock!" << endl;
		return;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket!" << endl;
		WSACleanup();
		return;
	}

	sockaddr_in server_side;
	server_side.sin_family = AF_INET;
	server_side.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &server_side.sin_addr);

	int connect_result = connect(sock, (sockaddr*)&server_side, sizeof(server_side));
	if (connect_result == SOCKET_ERROR)
	{
		cerr << "Can't connect to server\nClosing the created socket!" << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	char buffer_store[4096];
	string userInput;
	do
	{
		cout << "\nInput the message: ";
		getline(cin, userInput);
		if (userInput == "Quit" || userInput == "quit") {
			cout << "\nYou will quit the application, press any key." << endl;
			send(sock, userInput.c_str(), userInput.size() + 1, 0);
			closesocket(sock);
			WSACleanup();
			main();
		}
		cout << "\nYou say: " << userInput << endl;
		if (userInput.size() > 0)
		{
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buffer_store, 4096);
				int bytesReceived = recv(sock, buffer_store, 4096, 0);
				if (string(buffer_store, 0, bytesReceived) == "Quit" || string(buffer_store, 0, bytesReceived) == "quit") {
					char choose;
					cout << endl << inet_ntoa(server_side.sin_addr) << " is left.\nDo you want to re-connect?(Y/N):";
					cin >> choose;
					if (choose == 'Y') {
						closesocket(sock);
						WSACleanup();
						main();
					}
					else if (choose == 'N') {
						closesocket(sock);
						WSACleanup();
						cout << endl;
						exit(1);
					}
				}
				else if (bytesReceived > 0)
				{
					cout << endl << inet_ntoa(server_side.sin_addr) << " says: " << string(buffer_store, 0, bytesReceived) << endl;
				}
			}
		}

	} while (userInput.size() > 0);

	closesocket(sock);
	WSACleanup();
}


void server(string ip, int port) {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	cout << "\nWait for connecting..." << endl;
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Server can't set up correctly!" << endl;
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in server_side;
	server_side.sin_family = AF_INET;
	server_side.sin_port = htons(port);
	server_side.sin_addr.S_un.S_addr = INADDR_ANY;


	if (bind(listening, (sockaddr*)&server_side, sizeof(server_side)) == SOCKET_ERROR) //bind socket (required before listening)
	{
		std::cout << "Unable to bind socket!n";
		WSACleanup();
		exit(1);
	}
	else
		cout << "Bind IP and port successfully!";

	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create listening!" << endl;
		return;
	}
	else
		cout << "\nStart listening...\n";


	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << "\nAccept connecting: " << inet_ntoa(client.sin_addr) << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	closesocket(listening);

	char buffer_store[4096];
	bool first=true;
	string server_input;
	char buffer_input[4096];
	while (true)
	{
		ZeroMemory(buffer_store, 4096);

		int bytesReceived = recv(clientSocket, buffer_store, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Can't receiver message, quitting" << endl;
			break;
		}

		if (string(buffer_store, 0, bytesReceived) == "Quit" || string(buffer_store, 0, bytesReceived) == "quit" || bytesReceived == 0) {
			char choose;
			cout << inet_ntoa(client.sin_addr) << " left, conversion disconnected!\n\nDo you want to start listening again?(Y/N): ";
			cin >> choose;
			if (choose == 'Y')
				server(ip, port);
			else if (choose == 'N') {
				closesocket(clientSocket);
				WSACleanup();
				exit(1);
			}
		}
		cout << endl << inet_ntoa(client.sin_addr) << " says: ";
		cout << string(buffer_store, 0, bytesReceived) << endl;
		cout << "\nPlease input message: ";
		cin.get(buffer_input);
		getline(cin, server_input);
		String outstring = buffer_input + server_input;
		if (outstring == "Quit") {
			cout << "You will quit the application, press any key.\n";
			send(clientSocket, outstring.c_str(), outstring.size() + 1, 0);
			closesocket(clientSocket);
			WSACleanup();
			exit(1);
		}
		cout << "\nYou say: " << outstring << endl;
		if (outstring.size() > 0)
			send(clientSocket, outstring.c_str(), outstring.size() + 1, 0);
	}

	closesocket(clientSocket);
	WSACleanup();


}


int main() {
	string local_machine, remote_ip, choose;int port;
	do {
		cout << "\n*** CMD TEXT MESSENGER ***\n";
		cout << "(C)onnect peer\n";
		cout << "(W)ait for the other peer connecting\n";
		cout << "(Q)uit\n";
		cout << "Please choose: ";
		cin >> choose;
		if (choose[0] == 'C' && choose.length() == 1) {
			cout << "\nRemote IP: ";
			cin >> remote_ip;
			cout << "\nand port: ";
			cin >> port;
			cin.clear();
			cin.ignore();
			cilent(remote_ip, port);
		}
		else if (choose[0] == 'W' && choose.length() == 1) {
			getIPAddress();
			bool check = false;
			cout << "\nInput local machine IP: ";
			cin >> local_machine;
			cout << "\nInput the port that you want to bind: ";
			cin >> port;
			for (int i = 0;i < ip_size;i++)
				if (local_machine == store[i])
					check = true;
			if (!check) {
				cout << "\nCan't set up the server, the local machine IP is not valid!\n\n";
				exit(1);
			}
			server(local_machine, port);
		}
		else if (choose[0] == 'Q' && choose.length() == 1) {
			exit(1);
		}
		else {
			cout << "The input is wrong, input again!\n\n";
		}
	} while (true);
}