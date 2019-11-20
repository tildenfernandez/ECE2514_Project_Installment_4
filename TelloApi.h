#include <string>
#include <winsock2.h>


// Tello class version 1.2


// Tello move commands use relative centimeter displacements from the previous position.
// Coordinates can be positive or negative.
// Unlike AirSim, NED coordinates are *not* used.

struct Coordinates
{
	int x;
	int y;
	int z;
};


// A very basic class to communicate with and control a Tello drone.

class Tello
{
public:

	bool canInitialize();
	void arm();
	void takeoff();
	void move(Coordinates point);
	void land();

	
private:

	void receiveThread();
	bool sendCommand(std::string command);

private:

	static const int TELLO_SPEED = 30;		// Tello's speed in cm/sec
	static const int MAX_TIMEOUT = 5;		// maximum number of seconds allowed for Tello responses
	static const int BUFFER_SIZE = 256;		// Tello send and receive buffer sizes

	char send_buffer[BUFFER_SIZE];			// used for sending commands to the Tello
	char receive_buffer[BUFFER_SIZE];		// used for receiving responses from the Tello

	WSADATA wsa_data;						// used by WSAStartup to indicate the use of sockets
	SOCKET  socket_tello;					// socket for connecting to Tello

	sockaddr_in client_addr = {};			// structure for generating client internet address
	sockaddr_in server_addr = {};			// structure for generating server internet address
	sockaddr_in tello_addr  = {};			// used by receiveThread()'s recvfrom() call
};
