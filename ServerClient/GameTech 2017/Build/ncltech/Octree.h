/*
* Created for the purpose of dividing the scene into smaller parts and grouping the objects in it
* for culling purposes and better performance
*
**/

#pragma once
#include "PhysicsNode.h"
#include <vector>

using namespace std;

class Octree {
public:
	Octree(Vector3 c, float hdim, Octree *parent);
	~Octree();

	//Each object in the scene passes this check to see if it belongs to this octree branch
	void belongs(PhysicsNode *pnode);
	//Sorts the children of the octree into the branches
	void sortOct();
	//Returns true if the two objects should pair up
	bool checkPair(PhysicsNode *pnodeA, PhysicsNode *pnodeB);
	//Clears the children vector
	void clearChildren();

private:
	Octree* flu; //Front Left Up
	Octree* fld; //Front Left Down
	Octree* fru; //Front Right Up
	Octree* frd; //Front Right Down
	Octree* blu; //Back Left Up
	Octree* bld; //Back Left Down
	Octree* bru; //Back Right Up
	Octree* brd; //Back Right Down

	Octree* Parent;

	vector<PhysicsNode*> children;

	int depth = 0;
	float half_dimension;

	Vector3 centre;
};