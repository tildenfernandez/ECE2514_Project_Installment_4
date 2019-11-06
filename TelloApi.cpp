#include "pch.h"
#include <iostream>
#include <thread>
#include <ws2tcpip.h>
#include "TelloApi.h"

#pragma comment(lib, "ws2_32.lib")


// Tello class version 1.2

// This class provides a C++ class interface to communicate with the Tello drone using UDP.
// A socket implements the communication endpoint.
// See the Tello SDK 2.0 User Guide for a complete list of commands supported by the drone.


using namespace std;


// Try to establish two-way socket communication with the Tello and return whether this succeeds.
// Diagnostic messages are written to the console if communication cannot be established.

bool Tello::canInitialize()
{
	const char LOCAL_IP[] = "";					// laptop IP address
	const char TELLO_IP[] = "192.168.10.1";		// Tello IP address

	const u_short LOCAL_PORT = 8889;			// laptop UDP port
	const u_short TELLO_PORT = 8889;			// Tello UDP port

	// All processes (applications or DLLs) that call Winsock functions must initialize
	// the use of the Windows Sockets DLL before making other Winsock functions calls.
	
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

	if (result != 0) {
		cout << "Tello connection WSAStartup failed (error code " << result << ')' << endl;
		return false;
	}

	// Define the client address structure.

	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(LOCAL_PORT);
	inet_pton(AF_INET, LOCAL_IP, &client_addr.sin_addr.s_addr);

	// Define the server address structure.

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(TELLO_PORT);
	inet_pton(AF_INET, TELLO_IP, &server_addr.sin_addr.s_addr);

	// Create a socket.

	socket_tello = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (socket_tello == INVALID_SOCKET) {
		cout << "Error creating Tello socket (error code " << WSAGetLastError() << ')' << endl;
		return false;
	}

	// Bind the socket to the local address and port.

	if (::bind(socket_tello, (sockaddr *)&client_addr, sizeof(client_addr)) == SOCKET_ERROR) {
		cout << "Binding of Tello socket failed (error code " << WSAGetLastError() << ')' << endl;
		return false;
	}

	// Start the receive thread.

	thread receive_thread_tello(&Tello::receiveThread, this);

	// Detach the receive thread so it runs concurrently with the main thread.

	receive_thread_tello.detach();

	// Check if the Tello is responding to commands.

	if (!sendCommand("command")) {
		cout << "Tello not responding - is your laptop connected to it?" << endl;
		return false;
	}

	return true;
}


// The Tello has no explicit "arm" command, but send a "speed" command instead.

void Tello::arm()
{
	string speed_string = "speed " + to_string(TELLO_SPEED);
	sendCommand(speed_string);
}


// Send a "takeoff" command to the Tello.

void Tello::takeoff()
{
	sendCommand("takeoff");
}


// Send a "move" command to the Tello using the relative coordinates provided.
// Coordinates are relative, in units of centimeters, and can be positive or negative.

void Tello::move(Coordinates point)
{
	string command = "go " + to_string(point.x) + " " + to_string(point.y) + " "
		                   + to_string(point.z) + " " + to_string(TELLO_SPEED);

	sendCommand(command);
}


// Send a "land" command to the Tello.

void Tello::land()
{
	sendCommand("land");
}


// Send the string parameter to the Tello.
// The function returns true if the Tello responds with "ok" within MAX_TIMEOUT seconds;
// otherwise a diagnostic message is written to the console and the function returns false.

bool Tello::sendCommand(std::string command)
{
	cout << "Sending \"" << command << "\" to Tello" << endl;

	// Clear out the receive buffer.

	memset(receive_buffer, 0, sizeof(receive_buffer));

	// Convert the send command to type char* and convert to UTF-8 using the sendto() function.

	strcpy_s(send_buffer, sizeof(send_buffer), command.c_str());

	if (sendto(socket_tello, send_buffer, (int)strlen(send_buffer), 0,
		(sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		cout << "Failed to send \"" << command << "\" to Tello (error code "
			 << WSAGetLastError() << ')' << endl;
		return false;
	}

	// Wait for maximum timeout when awaiting response from Tello.

	clock_t start_time = clock();

	while (receive_buffer[1] == '\0') {
		if (((clock() - start_time) / CLOCKS_PER_SEC) > MAX_TIMEOUT) {
			cout << "Tello did not respond to \"" << command << "\" after "
				 << MAX_TIMEOUT << " seconds" << endl;
			return false;
		}
	}

	// Ensure that the Tello responds with "ok" rather than "error".

	if (strcmp(receive_buffer, "ok") != 0) {
		cout << "Tello returned \"" << receive_buffer << "\" for \"" << command << "\" command" << endl;
		return false;
	}
	
	return true;
}


// This function runs as a separate thread from the main() thread to receive and record
// responses from the Tello.

void Tello::receiveThread()
{
	memset(receive_buffer, 0, sizeof(receive_buffer));

	int tello_addr_length = sizeof(tello_addr);

	while (true) {
		if (recvfrom(socket_tello, receive_buffer, sizeof(receive_buffer), 0,
			(sockaddr *)&tello_addr, &tello_addr_length) == SOCKET_ERROR) {
			cout << "Failed to receive Tello response (error code "
				 << WSAGetLastError() << ')' << endl;
		}
	}
}
