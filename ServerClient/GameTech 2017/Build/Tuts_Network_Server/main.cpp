
/******************************************************************************
Class: Net1_Client
Implements:
Author: Pieran Marris <p.marris@newcastle.ac.uk> and YOU!
Description:

:README:
- In order to run this demo, we also need to run "Tuts_Network_Client" at the same time.
- To do this:-
	1. right click on the entire solution (top of the solution exporerer) and go to properties
	2. Go to Common Properties -> Statup Project
	3. Select Multiple Statup Projects
	4. Select 'Start with Debugging' for both "Tuts_Network_Client" and "Tuts_Network_Server"

- Now when you run the program it will build and run both projects at the same time. =]
- You can also optionally spawn more instances by right clicking on the specific project
and going to Debug->Start New Instance.




FOR MORE NETWORKING INFORMATION SEE "Tuts_Network_Client -> Net1_Client.h"



		(\_/)
		( '_')
	 /""""""""""""\=========     ----D
	/"""""""""""""""""""""""\
....\_@____@____@____@____@_/

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <enet\enet.h>
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>

#include "MazeGenerator.h"
#include "MazeRenderer.h"
#include "SearchAStar.h"
#include <nclgl\OBJMesh.h>
#include <deque>
#include <list>
#include <unordered_map>

//Needed to get computer adapter IPv4 addresses via windows
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")


#define SERVER_PORT 1234
#define UPDATE_TIMESTEP (1.0f / 30.0f) //send 30 position updates per second

enum msg { dens, g_size, grid, message };

//for sending packages of int
struct P_Data_int {
	msg m;
	int i;
};

//for sending packages with grid info
struct P_Data_grid {
	msg m;
	uint s;
	Vector3 st, en;

	vector<int> grid;
};

//for sending error messages or handle any other form of communication
struct P_Data_msg {
	msg m;
	char* messg;
};

NetworkBase server;
GameTimer timer;
float accum_time = 0.0f;
float rotation = 0.0f;
int density = 0, grid_size = 0;
bool flag = false;

MazeGenerator* generator;

void Win32_PrintAllAdapterIPAddresses();

int onExit(int exitcode)
{
	server.Release();
	system("pause");
	exit(exitcode);
}

int main(int arcg, char** argv)
{
	srand(93225);

	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	//Initialize Server on Port 1234, with a possible 32 clients connected at any time
	if (!server.Initialize(SERVER_PORT, 32))
	{
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		onExit(EXIT_FAILURE);
	}

	printf("Server Initiated\n");

	generator = new MazeGenerator();

	Win32_PrintAllAdapterIPAddresses();

	timer.GetTimedMS();
	while (true)
	{
		float dt = timer.GetTimedMS() * 0.001f;
		accum_time += dt;
		rotation += 0.5f * PI * dt;

		//Handle All Incoming Packets and Send any enqued packets
		server.ServiceNetwork(dt, [&](const ENetEvent& evnt)
		{
			switch (evnt.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("- New Client Connected\n");
				break;

			case ENET_EVENT_TYPE_RECEIVE:

				if ((int)*evnt.packet->data == dens) {
					P_Data_int *p = (P_Data_int*)evnt.packet->data;

					density = p->i;

					std::cout << density << std::endl;
					
					//Genrate new grid each time new params are sent
					if (grid_size != 0) {
						flag = true;
					}
				}
				else if ((int)*evnt.packet->data == g_size) {
					P_Data_int *p = (P_Data_int*)evnt.packet->data;

					grid_size = p->i;

					std::cout << grid_size << std::endl;

					//Genrate new grid each time new params are sent
					if (density != 0) {
						flag = true;
					}
				}
				else if ((int)*evnt.packet->data == message) {
					/*handle appropriately*/
				}

				enet_packet_destroy(evnt.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("- Client %d has disconnected.\n", evnt.peer->incomingPeerID);
				break;
			}
		});
		
		//Broadcast update packet to all connected clients at a rate of UPDATE_TIMESTEP updates per second
		if (accum_time >= UPDATE_TIMESTEP)
		{

			//Packet data
			// - At the moment this is just a position update that rotates around the origin of the world
			//   though this can be any variable, structure or class you wish. Just remember that everything 
			//   you send takes up valuable network bandwidth so no sending every PhysicsObject struct each frame ;)
			accum_time = 0.0f;

			if (flag) {

				generator->Generate(grid_size, (float)density);
				generator->genBoolGrid();

				P_Data_grid g;

				g.m = grid;
				g.s = generator->GetSize();
				g.st = generator->GetStartNode()->_pos;
				g.en = generator->GetGoalNode()->_pos;
				g.grid = generator->getGrid();

				int arraySize = (sizeof(msg) + sizeof(uint) + 2 * sizeof(g.st)) / sizeof(int) + g.grid.size();
				int* array = new int[arraySize];
				memcpy(array, &g, sizeof(msg) + sizeof(uint) + 2 * sizeof(g.st));
				int actualGrid = (sizeof(msg) + sizeof(uint) + 2 * sizeof(g.st)) / sizeof(int);

				for (int i = 0; i < g.grid.size(); ++i)
				{
					array[actualGrid + i] = g.grid[i];
				}
				
				ENetPacket* grid = enet_packet_create(array, arraySize * sizeof(int), 0);
				enet_host_broadcast(server.m_pNetwork, 0, grid);

				flag = false;
			}

		}

		Sleep(0);
	}

	system("pause");
	server.Release();
}




//Yay Win32 code >.>
//  - Grabs a list of all network adapters on the computer and prints out all IPv4 addresses associated with them.
void Win32_PrintAllAdapterIPAddresses()
{
	//Initially allocate 5KB of memory to store all adapter info
	ULONG outBufLen = 5000;
	

	IP_ADAPTER_INFO* pAdapters = NULL;
	DWORD status = ERROR_BUFFER_OVERFLOW;

	//Keep attempting to fit all adapter info inside our buffer, allocating more memory if needed
	// Note: Will exit after 5 failed attempts, or not enough memory. Lets pray it never comes to this!
	for (int i = 0; i < 5 && (status == ERROR_BUFFER_OVERFLOW); i++)
	{
		pAdapters = (IP_ADAPTER_INFO *)malloc(outBufLen);
		if (pAdapters != NULL) {

			//Get Network Adapter Info
			status = GetAdaptersInfo(pAdapters, &outBufLen);

			// Increase memory pool if needed
			if (status == ERROR_BUFFER_OVERFLOW) {
				free(pAdapters);
				pAdapters = NULL;
			}
			else {
				break;
			}
		}
	}

	
	if (pAdapters != NULL)
	{
		//Iterate through all Network Adapters, and print all IPv4 addresses associated with them to the console
		// - Adapters here are stored as a linked list termenated with a NULL next-pointer
		IP_ADAPTER_INFO* cAdapter = &pAdapters[0];
		while (cAdapter != NULL)
		{
			IP_ADDR_STRING* cIpAddress = &cAdapter->IpAddressList;
			while (cIpAddress != NULL)
			{
				printf("\t - Listening for connections on %s:%u\n", cIpAddress->IpAddress.String, SERVER_PORT);
				cIpAddress = cIpAddress->Next;
			}
			cAdapter = cAdapter->Next;
		}

		free(pAdapters);
	}
	
}