#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SpringConstraint.h>
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
		
		for (int i = 0; i < CSIZE; i++) {
			for (int j = 0; j < CSIZE; j++) {
				ball = CommonUtils::BuildSphereObject("",
					Vector3(i*0.5 - CSIZE/2, 5, j*0.5 - CSIZE/2),				//Position
					0.1f,									//Radius
					true,									//Has Physics Object
					i==0 && (j == 0 || j == CSIZE - 1) ? 0.0f : 1.0f,									// Inverse Mass
					true,									//No Collision Shape Yet
					true,									//Dragable by the user
					CommonUtils::GenColor(0.5f, 1.0f));		//Color

				s.push_back(ball);
			}
		}

		//Distance Constraint

		/*SpringConstraint* constraint;

		for (int i = 0; i < CSIZE; i++) {
			for (int j = 0; j < CSIZE; j++) {
				if (i - 1 >= 0) {
					constraint = new SpringConstraint(
						s[i * CSIZE + j]->Physics(),					//Physics Object A
						s[(i - 1) * CSIZE + j]->Physics(),					//Physics Object B
						s[i * CSIZE + j]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i - 1) * CSIZE + j]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}

				if (j - 1 >= 0) {
					constraint = new SpringConstraint(
						s[i * CSIZE + j]->Physics(),					//Physics Object A
						s[(i) * CSIZE + (j-1)]->Physics(),					//Physics Object B
						s[i * CSIZE + j]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i) * CSIZE + (j-1)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}

				if (i - 1 >= 0 && j - 1 >= 0) {
					constraint = new SpringConstraint(
						s[i * CSIZE + j]->Physics(),					//Physics Object A
						s[(i - 1) * CSIZE + (j - 1)]->Physics(),					//Physics Object B
						s[i * CSIZE + j]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i - 1) * CSIZE + (j - 1)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}
			}
		}*/


		//Spring Constraint

		SpringConstraint* constraint;

		for (int i = 0; i < CSIZE; i++) {
			for (int j = 0; j < CSIZE; j++) {
				if (i - 1 >= 0) {
					constraint = new SpringConstraint(
						s[i * CSIZE + j]->Physics(),					//Physics Object A
						s[(i - 1) * CSIZE + j]->Physics(),					//Physics Object B
						s[i * CSIZE + j]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i - 1) * CSIZE + j]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}

				if (j - 1 >= 0) {
					constraint = new SpringConstraint(
						s[i * CSIZE + j]->Physics(),					//Physics Object A
						s[(i)* CSIZE + (j - 1)]->Physics(),					//Physics Object B
						s[i * CSIZE + j]->Physics()->GetPosition(),	//Attachment Position on Object A	-> Currently the centre
						s[(i)* CSIZE + (j - 1)]->Physics()->GetPosition());	//Attachment Position on Object B	-> Currently the centre  
					PhysicsEngine::Instance()->AddConstraint(constraint);
				}

				if (i - 1 >= 0 && j - 1 >= 0) {
					constraint = new SpringConstraint(
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

	virtual void OnCleanupScene() override
	{
		Scene::OnCleanupScene();
		s.clear();
	}

	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);

		//Draw Polygons between cloth nodes
		for (int i = 0; i < CSIZE - 1; i++) {
			for (int j = 0; j < CSIZE - 1; j++) {

				Vector3 *v = new Vector3[4];

				v[0] = (s[i*CSIZE + j]->Physics()->GetPosition());
				v[1] = (s[i*CSIZE + (j + 1)]->Physics()->GetPosition());
				v[2] = (s[(i + 1)*CSIZE + (j + 1)]->Physics()->GetPosition());
				v[3] = (s[(i+1)*CSIZE + j]->Physics()->GetPosition());

				NCLDebug::DrawPolygon(4, v, Vector4(0.7f, 0.3f, 0.2f, 0.5f));

				v[0] = (s[i*CSIZE + j]->Physics()->GetPosition());
				v[1] = (s[(i + 1)*CSIZE + j]->Physics()->GetPosition());
				v[2] = (s[(i + 1)*CSIZE + (j + 1)]->Physics()->GetPosition());
				v[3] = (s[i*CSIZE + (j + 1)]->Physics()->GetPosition());

				NCLDebug::DrawPolygon(4, v, Vector4(0.7f, 0.3f, 0.2f, 0.5f));
			}
		}
		
		//Draw Constraint lines
		/*for (int i = 0; i < CSIZE; i++) {
			for (int j = 0; j < CSIZE; j++) {
				if (i - 1 >= 0) {
					NCLDebug::DrawThickLine(s[i * CSIZE + j]->Physics()->GetPosition(), s[(i - 1) * CSIZE + j]->Physics()->GetPosition(), 0.02, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
				}

				if (j - 1 >= 0) {
					NCLDebug::DrawThickLine(s[i * CSIZE + j]->Physics()->GetPosition(), s[(i)* CSIZE + (j - 1)]->Physics()->GetPosition(), 0.02, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
				}

				if (i - 1 >= 0 && j - 1 >= 0) {
					NCLDebug::DrawThickLine(s[i * CSIZE + j]->Physics()->GetPosition(), s[(i - 1)* CSIZE + (j - 1)]->Physics()->GetPosition(), 0.02, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
				}
			}
		}*/
	}

private:
	GameObject* ball;
	vector<GameObject*> s;
};