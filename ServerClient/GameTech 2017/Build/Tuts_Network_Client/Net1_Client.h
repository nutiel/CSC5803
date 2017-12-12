
#pragma once

#include <ncltech\Scene.h>
#include <ncltech\NetworkBase.h>

#include "MazeGenerator.h"
#include "MazeRenderer.h"
#include "SearchAStar.h"
#include <nclgl\OBJMesh.h>
#include <deque>
#include <list>
#include <unordered_map>

//Basic Network Example

class Net1_Client : public Scene
{
public:
	Net1_Client(const std::string& friendly_name);

	virtual ~Net1_Client() {
		SAFE_DELETE(wallmesh);
	}

	virtual void OnInitializeScene() override;
	virtual void OnCleanupScene() override;
	virtual void OnUpdateScene(float dt) override;

	void GenerateNewMaze();

	void ProcessNetworkEvent(const ENetEvent& evnt);

	void UpdateAStarPreset();

protected:
	GameObject* box;

	NetworkBase network;
	ENetPeer* serverConnection;

	MazeGenerator* generator;
	MazeRenderer* maze;
	SearchAStar* search_as;

	int grid_size;
	float density;
	int astar_preset_idx;
	std::string astar_preset_text;

	Mesh* wallmesh;
};