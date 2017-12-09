#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <nclgl\NCLDebug.h>

#define WIDTH 20.0f
#define HEIGHT 40.0f
#define DEPTH 1.0f
#define FRICTION 0.0001f

//Fully striped back scene to use as a template for new scenes.
class CollisionScenarios : public Scene
{
public:
	CollisionScenarios(const std::string& friendly_name)
		: Scene(friendly_name)
	{
	}

	virtual ~CollisionScenarios()
	{
		SAFE_DELETE(obj);
	}

	virtual void OnInitializeScene() override
	{

		Scene::OnInitializeScene();

		GraphicsPipeline::Instance()->GetCamera()->SetPosition(Vector3(0.0f, 30.0f, 30.0f));
		GraphicsPipeline::Instance()->GetCamera()->SetPitch(-45.0f);
		GraphicsPipeline::Instance()->GetCamera()->SetYaw(0.0f);

		//Who doesn't love finding some common ground?
		obj = CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, -1.0f, 0.0f),
			Vector3(20.0f, 1.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		this->AddGameObject(obj);
		obj->Physics()->SetFriction(FRICTION);
		//Pool Walls
		{
			obj = CommonUtils::BuildCuboidObject(
				"Wall",
				Vector3(-WIDTH, HEIGHT, 0.0f),
				Vector3(DEPTH, HEIGHT, WIDTH),
				true,
				0.0f,
				true,
				false,
				Vector4(0.5f, 0.5f, 0.5f, 0.0f));
			obj->Physics()->SetOnCollisionCallback(&CollisionScenarios::CallbackFunctionWall_x);
			this->AddGameObject(obj);

			obj = CommonUtils::BuildCuboidObject(
				"Wall",
				Vector3(WIDTH, HEIGHT, 0.0f),
				Vector3(DEPTH, HEIGHT, WIDTH),
				true,
				0.0f,
				true,
				false,
				Vector4(0.5f, 0.5f, 0.5f, 0.0f));
			obj->Physics()->SetOnCollisionCallback(&CollisionScenarios::CallbackFunctionWall_x);
			this->AddGameObject(obj);

			obj = CommonUtils::BuildCuboidObject(
				"Wall",
				Vector3(0.0f, HEIGHT, -WIDTH),
				Vector3(WIDTH, HEIGHT, DEPTH),
				true,
				0.0f,
				true,
				false,
				Vector4(0.5f, 0.5f, 0.5f, 0.0f));
			obj->Physics()->SetOnCollisionCallback(&CollisionScenarios::CallbackFunctionWall_z);
			this->AddGameObject(obj);

			obj = CommonUtils::BuildCuboidObject(
				"Wall",
				Vector3(0.0f, HEIGHT, WIDTH),
				Vector3(WIDTH, HEIGHT, DEPTH),
				true,
				0.0f,
				true,
				false,
				Vector4(0.5f, 0.5f, 0.5f, 0.0f));
			obj->Physics()->SetOnCollisionCallback(&CollisionScenarios::CallbackFunctionWall_z);
			this->AddGameObject(obj);

		}


		//Objects
		{
			obj = CommonUtils::BuildCuboidObject(
				"cube1",
				Vector3(0.0f, 1.0f, 3.0f),
				Vector3(1.0f, 1.0f, 1.0f),
				true,
				1.0f,
				true,
				true,
				Vector4(0.7f, 0.5f, 0.3f, 1.0f));
			this->AddGameObject(obj);
			s.push_back(obj);

			obj = CommonUtils::BuildCuboidObject(
				"cube2",
				Vector3(3.0f, 1.0f, 3.0f),
				Vector3(1.0f, 1.0f, 1.0f),
				true,
				1.0f,
				true,
				true,
				Vector4(0.3f, 0.5f, 0.7f, 1.0f));
			this->AddGameObject(obj);
			s.push_back(obj);

			obj = CommonUtils::BuildSphereObject(
				"sphere1",
				Vector3(3.0f, 1.0f, 0.0f),
				1.0f,
				true,
				1.0f,
				true,
				true,
				Vector4(0.2f, 0.6f, 0.2f, 1.0f));
			this->AddGameObject(obj);
			s.push_back(obj);

			obj = CommonUtils::BuildSphereObject(
				"sphere2",
				Vector3(0.0f, 1.0f, 0.0f),
				1.0f,
				true,
				1.0f,
				true,
				true,
				Vector4(0.9f, 0.1f, 0.5f, 1.0f));
			this->AddGameObject(obj);
			s.push_back(obj);

			double vx, vz;

			for (int i = 0; i < s.size(); i++) {
				vx = ((double)rand() / (RAND_MAX/2.0 + 1.0)) - 1.0;
				vz = ((double)rand() / (RAND_MAX/2.0 + 1.0)) - 1.0;
				

				s[i]->Physics()->SetLinearVelocity(Vector3(vx*50, 0.0f, vz*50));
				s[i]->Physics()->SetFriction(FRICTION);
			}
		}
	}

	//Turns the walls into buffers (similar to pinball) for the vertical wall
	static bool CallbackFunctionWall_x(PhysicsNode* self, PhysicsNode* collidingObject) {

		NCLDebug::Log(Vector3(0.3f, 1.0f, 0.3f), "Wall Hit!!! --> X Axis");

		Vector3 v = collidingObject->GetLinearVelocity();
		v.x *= -1;

		v.x = v.x < 0 ? max(-50.0, v.x*2.0) : min(50.0, v.x*2.0);
		v.z = v.z < 0 ? max(-50.0, v.z*2.0) : min(50.0, v.z*2.0);

		collidingObject->SetLinearVelocity(v);

		return false;
	}

	//Same as above for the horizontal walls
	static bool CallbackFunctionWall_z(PhysicsNode* self, PhysicsNode* collidingObject) {

		NCLDebug::Log(Vector3(0.3f, 1.0f, 0.3f), "Wall Hit!!! --> Z Axis");

		Vector3 v = collidingObject->GetLinearVelocity();
		v.z *= -1;

		v.x = v.x < 0 ? max(-50.0, v.x*2.0) : min(50.0, v.x*2.0);
		v.z = v.z < 0 ? max(-50.0, v.z*2.0) : min(50.0, v.z*2.0);

		collidingObject->SetLinearVelocity(v);

		return false;
	}

	float m_AccumTime;
	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);


		//Update Rotating Objects!
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))
		{
			m_AccumTime += dt*20.0f;

			float cost = cosf((float)DegToRad(m_AccumTime * 45.0f));
			float sint = sinf((float)DegToRad(m_AccumTime * 45.0f));

			/*Update objects here*/
		}

		//Add a random velocity to all the objects on scene
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F)) {
			double vx, vz, vxt, vzt;

			for (int i = 0; i < s.size(); i++) {
				vx = ((double)rand() / (RAND_MAX / 2.0 + 1.0)) - 1.0;
				vz = ((double)rand() / (RAND_MAX / 2.0 + 1.0)) - 1.0;

				vxt = vx < 0 ? max(-50.0, s[i]->Physics()->GetLinearVelocity().x + vx * 30) : min(50.0, s[i]->Physics()->GetLinearVelocity().x + vx * 30);
				vzt = vz < 0 ? max(-50.0, s[i]->Physics()->GetLinearVelocity().z + vz * 30) : min(50.0, s[i]->Physics()->GetLinearVelocity().z + vz * 30);

				s[i]->Physics()->SetLinearVelocity(Vector3(vxt, 0.0f, vzt));
			}
		}
	}

	virtual void OnCleanupScene() override
	{
		Scene::OnCleanupScene();
		s.clear();
	}

private:
	GameObject* obj;
	vector<GameObject*> s;
};