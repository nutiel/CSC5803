#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\Scene.h>
#include "SearchAlgorithm.h"



class MazeGenerator
{
public:
	MazeGenerator(); //Maze_density goes from 1 (single path to exit) to 0 (no walls at all)
	virtual ~MazeGenerator();

	void Generate(int size, float maze_density);

	//All points on the maze grid are connected in some shape or form
	// so any two nodes picked randomly /will/ have a path between them
	GraphNode* GetStartNode() const		{ return start; }
	GraphNode* GetGoalNode()  const		{ return end; }
	uint GetSize() const { return size; }

	GraphNode* getAllNodes() { return allNodes; }
	GraphEdge* getAllEdges() { return allEdges; }

	void setStartNode(GraphNode* startN) { start = startN; }
	void setGoalNode(GraphNode* endN) { end = endN; }

	void setSize(uint s) { size = s; }

	void setAllNodes(GraphNode* allN) { allNodes = allN; }
	void setAllEdges(GraphEdge* allE) { allEdges = allE; }

	void genBoolGrid() {
		grid = new bool[size*size];

		int a, b;

		for (uint i = 0; i < 2 * size * (size - 1); i++) {
			a = (int)(allEdges[i]._a->_pos.x + size*allEdges[i]._a->_pos.y);
			b = (int)(allEdges[i]._b->_pos.x + size*allEdges[i]._b->_pos.y);

			grid[a] = false;
			grid[a] = false;

			if (!allEdges[i]._iswall) {
				grid[a] = true;
				grid[b] = true;
			}
		}
	}

	vector<int> getGrid() {
		vector<int> v;
		for (uint i = 0; i < size*size; i++) {
			v.push_back(grid[i]);
		}
		return v;
	}

	//Used as a hack for the MazeRenderer to generate the walls more effeciently
	GraphNode* GetAllNodesArr() { return allNodes; }

protected:
	void GetRandomStartEndNodes();

	void Initiate_Arrays();

	void Generate_Prims();
	void Generate_Sparse(float density);

	

public:
	uint size;
	GraphNode *start, *end;

	bool* grid;

	GraphNode* allNodes;
	GraphEdge* allEdges;
};