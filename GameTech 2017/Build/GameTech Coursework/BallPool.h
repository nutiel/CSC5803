#pragma once

#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <nclgl\NCLDebug.h>

#define WIDTH 3.0f
#define HEIGHT 3.5f
#define DEPTH 0.5f

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
			Vector4(0.2f, 0.5f, 1.0f, 0.1f)));
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Wall",
			Vector3(WIDTH, HEIGHT, 0.0f),
			Vector3(DEPTH, HEIGHT, WIDTH),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 0.1f)));
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Wall",
			Vector3(0.0f, HEIGHT, -WIDTH),
			Vector3(WIDTH, HEIGHT, DEPTH),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 0.1f)));
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Wall",
			Vector3(0.0f, HEIGHT, WIDTH),
			Vector3(WIDTH, HEIGHT, DEPTH),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 0.1f)));

		for (int i = 0; i < 100; i++) {
			this->AddGameObject(CommonUtils::BuildSphereObject(
				"",
				Vector3(0.0f, 10.0f + i, 0.0f),
				0.5f,
				true,
				1.0f,
				true,
				true,
				Vector4(0.8f, i>33?0.6:0.1, i<66?0.6:0.2, 1.0f)
			));
		}
	}
};