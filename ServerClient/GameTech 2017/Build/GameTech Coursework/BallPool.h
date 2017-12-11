#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <nclgl\NCLDebug.h>

#define WIDTH 1.7f
#define HEIGHT 3.0f
#define DEPTH 0.25f

//Fully striped back scene to use as a template for new scenes.
class BallPool : public Scene
{
public:
	BallPool(const std::string& friendly_name)
		: Scene(friendly_name)
	{
	}

	virtual ~BallPool()
	{

	}

	virtual void OnInitializeScene() override
	{

		Scene::OnInitializeScene();

		//Who doesn't love finding some common ground?
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, -1.0f, 0.0f),
			Vector3(20.0f, 1.0f, 20.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

		//Pool Walls
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Wall",
			Vector3(-WIDTH, HEIGHT, 0.0f),
			Vector3(DEPTH, HEIGHT, WIDTH),
			true,
			0.0f,
			true,
			false,
			Vector4(0.5f, 0.5f, 0.5f, 0.1f)));
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Wall",
			Vector3(WIDTH, HEIGHT, 0.0f),
			Vector3(DEPTH, HEIGHT, WIDTH),
			true,
			0.0f,
			true,
			false,
			Vector4(0.5f, 0.5f, 0.5f, 0.1f)));
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Wall",
			Vector3(0.0f, HEIGHT, -WIDTH),
			Vector3(WIDTH, HEIGHT, DEPTH),
			true,
			0.0f,
			true,
			false,
			Vector4(0.5f, 0.5f, 0.5f, 0.1f)));
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Wall",
			Vector3(0.0f, HEIGHT, WIDTH),
			Vector3(WIDTH, HEIGHT, DEPTH),
			true,
			0.0f,
			true,
			false,
			Vector4(0.5f, 0.5f, 0.5f, 0.1f)));

		for (int i = 0; i < 100; i++) {
			this->AddGameObject(CommonUtils::BuildSphereObject(
				"",
				Vector3(0.0f, 10.0f + i, 0.0f),
				0.3f,
				true,
				0.5f,
				true,
				true,
				Vector4(0.8f, i>33?0.6:0.1, i<66?0.6:0.2, 1.0f)
			));
		}
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
	}
};