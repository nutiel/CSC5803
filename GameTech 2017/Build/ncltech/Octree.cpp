#include "Octree.h"
#include <nclgl\NCLDebug.h>

#define THRESHHOLD 15

Octree::Octree(Vector3 c, float hdim, Octree *parent) :
	centre(c),
	half_dimension(hdim),
	Parent(parent) {

	if (Parent != NULL) {
		depth = Parent->depth + 1;
	}
	else {
		depth = 0;
	}

	flu = NULL;
	fld = NULL;
	fru = NULL;
	frd = NULL;
	blu = NULL;
	bld = NULL;
	bru = NULL;
	brd = NULL;
}

Octree::~Octree() {
	SAFE_DELETE(flu);
	SAFE_DELETE(fld);
	SAFE_DELETE(fru);
	SAFE_DELETE(frd);
	SAFE_DELETE(blu);
	SAFE_DELETE(bld);
	SAFE_DELETE(brd);
	SAFE_DELETE(brd);

	children.clear();
}

void Octree::belongs(PhysicsNode *pnode) {

	Vector3 p = pnode->GetPosition();
	float b = pnode->getBoundingRad();

	if (p.x < (this->centre.x + half_dimension + b) && p.x > (this->centre.x - half_dimension - b)
		&& p.y < (this->centre.y + half_dimension + b) && p.y > (this->centre.y - half_dimension - b)
		&& p.z < (this->centre.z + half_dimension + b) && p.z > (this->centre.z - half_dimension - b)) {

		//Add to this part of the octree
		children.push_back(pnode);
		}
}

void Octree::sortOct() {

	//Show octs//

	/*NCLDebug::DrawThickLine(Vector3(centre.x - half_dimension, centre.y + half_dimension, centre.z - half_dimension),
		Vector3(centre.x + half_dimension, centre.y + half_dimension, centre.z - half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //1-2
	NCLDebug::DrawThickLine(Vector3(centre.x - half_dimension, centre.y + half_dimension, centre.z - half_dimension),
		Vector3(centre.x - half_dimension, centre.y + half_dimension, centre.z + half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //1-4
	NCLDebug::DrawThickLine(Vector3(centre.x + half_dimension, centre.y + half_dimension, centre.z + half_dimension),
		Vector3(centre.x + half_dimension, centre.y + half_dimension, centre.z - half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //3-2
	NCLDebug::DrawThickLine(Vector3(centre.x + half_dimension, centre.y + half_dimension, centre.z + half_dimension),
		Vector3(centre.x - half_dimension, centre.y + half_dimension, centre.z + half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //3-4

	NCLDebug::DrawThickLine(Vector3(centre.x - half_dimension, centre.y - half_dimension, centre.z - half_dimension),
		Vector3(centre.x - half_dimension, centre.y - half_dimension, centre.z + half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //5-8
	NCLDebug::DrawThickLine(Vector3(centre.x - half_dimension, centre.y - half_dimension, centre.z + half_dimension),
		Vector3(centre.x + half_dimension, centre.y - half_dimension, centre.z + half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //8-7
	NCLDebug::DrawThickLine(Vector3(centre.x - half_dimension, centre.y - half_dimension, centre.z - half_dimension),
		Vector3(centre.x + half_dimension, centre.y - half_dimension, centre.z - half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //5-6
	NCLDebug::DrawThickLine(Vector3(centre.x + half_dimension, centre.y - half_dimension, centre.z + half_dimension),
		Vector3(centre.x + half_dimension, centre.y - half_dimension, centre.z - half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //7-6

	NCLDebug::DrawThickLine(Vector3(centre.x - half_dimension, centre.y - half_dimension, centre.z - half_dimension),
		Vector3(centre.x - half_dimension, centre.y + half_dimension, centre.z - half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //5-1
	NCLDebug::DrawThickLine(Vector3(centre.x - half_dimension, centre.y + half_dimension, centre.z + half_dimension),
		Vector3(centre.x - half_dimension, centre.y - half_dimension, centre.z + half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //4-8
	NCLDebug::DrawThickLine(Vector3(centre.x + half_dimension, centre.y + half_dimension, centre.z - half_dimension),
		Vector3(centre.x + half_dimension, centre.y - half_dimension, centre.z - half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //2-6
	NCLDebug::DrawThickLine(Vector3(centre.x + half_dimension, centre.y + half_dimension, centre.z + half_dimension),
		Vector3(centre.x + half_dimension, centre.y - half_dimension, centre.z + half_dimension), 0.01, Vector4(0.0f, 0.0f, 0.0f, 1.0f)); //3-7*/

	//Show octs//

	if (children.size() > THRESHHOLD && depth < 4) {
		if (flu == NULL) {
			flu = new Octree(Vector3(centre.x - half_dimension / 2, centre.y + half_dimension / 2, centre.z + half_dimension / 2), half_dimension / 2, this);
			fld = new Octree(Vector3(centre.x - half_dimension / 2, centre.y - half_dimension / 2, centre.z + half_dimension / 2), half_dimension / 2, this);
			fru = new Octree(Vector3(centre.x + half_dimension / 2, centre.y + half_dimension / 2, centre.z + half_dimension / 2), half_dimension / 2, this);
			frd = new Octree(Vector3(centre.x + half_dimension / 2, centre.y - half_dimension / 2, centre.z + half_dimension / 2), half_dimension / 2, this);
			blu = new Octree(Vector3(centre.x - half_dimension / 2, centre.y + half_dimension / 2, centre.z - half_dimension / 2), half_dimension / 2, this);
			bld = new Octree(Vector3(centre.x - half_dimension / 2, centre.y - half_dimension / 2, centre.z - half_dimension / 2), half_dimension / 2, this);
			bru = new Octree(Vector3(centre.x + half_dimension / 2, centre.y + half_dimension / 2, centre.z - half_dimension / 2), half_dimension / 2, this);
			brd = new Octree(Vector3(centre.x + half_dimension / 2, centre.y - half_dimension / 2, centre.z - half_dimension / 2), half_dimension / 2, this);
		}

		for (size_t i = 0; i < children.size(); ++i)
		{
			PhysicsNode *pnode = children.at(i);

			flu->belongs(pnode);
			fld->belongs(pnode);
			fru->belongs(pnode);
			frd->belongs(pnode);
			blu->belongs(pnode);
			bld->belongs(pnode);
			bru->belongs(pnode);
			brd->belongs(pnode);
		}

		flu->sortOct();
		fld->sortOct();
		fru->sortOct();
		frd->sortOct();
		blu->sortOct();
		bld->sortOct();
		bru->sortOct();
		brd->sortOct();
	}
	else if (flu != NULL) {
		flu->sortOct();
		fld->sortOct();
		fru->sortOct();
		frd->sortOct();
		blu->sortOct();
		bld->sortOct();
		bru->sortOct();
		brd->sortOct();

		SAFE_DELETE(flu);
		SAFE_DELETE(fld);
		SAFE_DELETE(fru);
		SAFE_DELETE(frd);
		SAFE_DELETE(blu);
		SAFE_DELETE(bld);
		SAFE_DELETE(bru);
		SAFE_DELETE(brd);
	}
}

bool Octree::checkPair(PhysicsNode *pnodeA, PhysicsNode *pnodeB) {
	bool flag = false;

	//check if pnodeA belongs to this branch
	for (int i = 0; i < children.size(); i++) {
		if (children.at(i) == pnodeA) {
			flag = true;
		}
	}

	//if not found return false
	if (!flag)
		return false;

	//reset flag
	flag = false;

	//check if pnodeB belongs to this branch
	for (int i = 0; i < children.size(); ++i) {
		//break as soon as pnodeB is found
		if (children.at(i) == pnodeB) {
			flag = true;
			break;
		}
		else {
			flag = false;
		}
	}

	//if both nodes are found chech the child branches of this branch
	if (flag && flu != NULL) {

		flag = false; //reset flag

		if (flu->checkPair(pnodeA, pnodeB))
			flag = true;

		if (!flag && fld->checkPair(pnodeA, pnodeB))
			flag = true;
				
		if (!flag && fru->checkPair(pnodeA, pnodeB))
			flag = true;

		if (!flag && frd->checkPair(pnodeA, pnodeB))
			flag = true;

		if (!flag && blu->checkPair(pnodeA, pnodeB))
			flag = true;

		if (!flag && bld->checkPair(pnodeA, pnodeB))
			flag = true;

		if (!flag && bru->checkPair(pnodeA, pnodeB))
			flag = true;
					
		if (!flag && brd->checkPair(pnodeA, pnodeB))
			flag = true;

		return flag;
	}
	else if (flag) {
		return true; //if this is the leaf branches and both nodes are found return true
	}
	
	return false;
}

void Octree::clearChildren() {

	if (flu != NULL) {
		flu->clearChildren();
		fld->clearChildren();
		fru->clearChildren();
		frd->clearChildren();
		blu->clearChildren();
		bld->clearChildren();
		bru->clearChildren();
		brd->clearChildren();
	}

	children.clear();
}