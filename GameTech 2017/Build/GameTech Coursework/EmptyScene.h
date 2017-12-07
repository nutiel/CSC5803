#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <nclgl\NCLDebug.h>

#define CSIZE 10

//Fully striped back scene to use as a template for new scenes.
class EmptyScene : public Scene
{
public:
	EmptyScene(const std::string& friendly_name) 
		: Scene(friendly_name)
	{
	}

	virtual ~EmptyScene()
	{
		SAFE_DELETE(ball);
	}

	virtual void OnInitializeScene() override
	{

		Scene::OnInitializeScene();

		//Who doesn't love finding some common ground?
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, -1.5f, 0.0f),
			Vector3(20.0f, 1.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

		vector<GameObject*> s;
		
		for (int i = 0; i < CSIZE; i++) {
			for (int j = 0; j < CSIZE; j++) {
				ball = CommonUtils::BuildSphereObject("",
					Vector3(i*0.5 - CSIZE/2, 5, j*0.5 - CSIZE/2),				//Position
					0.1f,									//Radius
					true,									//Has Physics Object
					i==0 ? 0.0f : 1.0f,									// Inverse Mass
					true,									//No Collision Shape Yet
					true,									//Dragable by the user
					CommonUtils::GenColor(0.5f, 1.0f));		//Color

				s.push_back(ball);
			}
		}

		DistanceConstraint* constraint;

		for (int i = 0; i < CSIZE; i++) {
			for (int j = 0; j < CSIZE; j++) {
				if (i - 1 >= 0) {
					constraint = new DistanceConstraint(
						s[i * CSIZE + j]->Physics(),					//Physics Object A
						s[(i - 1) * CSIZE + j]->Physics(),					//Physics Object B
						s[i * CSIZE + j]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i - 1) * CSIZE + j]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}

				if (j - 1 >= 0) {
					constraint = new DistanceConstraint(
						s[i * CSIZE + j]->Physics(),					//Physics Object A
						s[(i) * CSIZE + (j-1)]->Physics(),					//Physics Object B
						s[i * CSIZE + j]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i) * CSIZE + (j-1)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}

				if (i - 1 >= 0 && j - 1 >= 0) {
					constraint = new DistanceConstraint(
						s[i * CSIZE + j]->Physics(),					//Physics Object A
						s[(i - 1) * CSIZE + (j - 1)]->Physics(),					//Physics Object B
						s[i * CSIZE + j]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i - 1) * CSIZE + (j - 1)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}
			}
		}

		for (int i = 0; i < s.size(); i++) {
			this->AddGameObject(s[i]);
		}
	}

private:
	GameObject* ball;
};