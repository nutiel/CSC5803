#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <nclgl\NCLDebug.h>

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

		s = new GameObject[4 * 4];

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				ball = CommonUtils::BuildSphereObject("",
					Vector3(i*1.5 - 2, 2, j*1.5 - 2),				//Position
					0.5f,									//Radius
					true,									//Has Physics Object
					1.0f,									// Inverse Mass
					true,									//No Collision Shape Yet
					true,									//Dragable by the user
					CommonUtils::GenColor(0.5f, 1.0f));		//Color
				s[i * 4 + j] = *ball;
			}
		}
		DistanceConstraint* constraint;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i - 1 >= 0) {
					constraint = new DistanceConstraint(
						s[i * 4 + j].Physics(),					//Physics Object A
						s[(i - 1) * 4 + j].Physics(),					//Physics Object B
						s[i * 4 + j].Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i - 1) * 4 + j].Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}

				if (j - 1 >= 0) {
					constraint = new DistanceConstraint(
						s[i * 4 + j].Physics(),					//Physics Object A
						s[(i) * 4 + (j-1)].Physics(),					//Physics Object B
						s[i * 4 + j].Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i) * 4 + (j-1)].Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}
			}
		}
		for (int i = 0; i < 4 * 4; i++) {
			this->AddGameObject(&s[i]);
		}
	}

private:
	GameObject* ball;
	GameObject *s;
};