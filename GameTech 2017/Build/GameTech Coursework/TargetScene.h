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
class TargetScene : public Scene
{
public:
	TargetScene(const std::string& friendly_name)
		: Scene(friendly_name)
	{
	}

	virtual ~TargetScene()
	{
		SAFE_DELETE(obj);
	}

	virtual void OnInitializeScene() override
	{

		Scene::OnInitializeScene();
		score = 0;

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

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (rand() > RAND_MAX/2) {
					this->AddGameObject(addGoodTarget(Vector3(i*1.5, j*1.5 + 1.0, 0.0f)));
				}
				else {
					this->AddGameObject(addBadTarget(Vector3(i*1.5, j*1.5 + 1.0, 0.0f)));
				}
			}
		}
		
	}

	GameObject* addGoodTarget(Vector3 p) {
		obj = CommonUtils::BuildCuboidObject(
			"GoodTarget",
			p,
			Vector3(0.5f, 0.5f, 0.5f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		obj->Physics()->SetOnCollisionCallback(
			std::bind(&TargetScene::CallbackFunction_T_Hit,
				this,							//Any non-placeholder param will be passed into the function each time it is called
				std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
				std::placeholders::_2
			)
		);
		return obj;
	}

	GameObject* addBadTarget(Vector3 p) {
		obj = CommonUtils::BuildCuboidObject(
			"BadTarget",
			p,
			Vector3(0.5f, 0.5f, 0.5f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		obj->Physics()->SetOnCollisionCallback(
			std::bind(&TargetScene::CallbackFunction_T_Hit,
				this,							//Any non-placeholder param will be passed into the function each time it is called
				std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
				std::placeholders::_2
			)
		);
		return obj;
	}

	//Turns the walls into buffers (similar to pinball) for the vertical wall
	bool CallbackFunction_T_Hit(PhysicsNode* self, PhysicsNode* collidingObject) {

		if (self->GetParent()->GetName() == "GoodTarget") {
			NCLDebug::Log(Vector3(0.3f, 1.0f, 0.3f), "Good Target hit");
			//Need new function to handle text in the onUpdateScene function and a structure to save texts to be shown (in case of multiples), position, time left before disappearing ect
			//NCLDebug::DrawTextWs(self->GetPosition()+Vector3(0.0f, 0.0f, 1.0f), 1.0f, TEXTALIGN_CENTRE, Vector4(0.0f, 0.0f, 0.0f, 1.0f), "+100 pts");
			score += 100;
		}
		else if(self->GetParent()->GetName() == "BadTarget"){
			NCLDebug::Log(Vector3(0.3f, 1.0f, 0.3f), "Bad Target hit");
			//same as above
			//NCLDebug::DrawTextWs(self->GetPosition() + Vector3(0.0f, 0.0f, 1.0f), 1.0f, TEXTALIGN_CENTRE, Vector4(0.0f, 0.0f, 0.0f, 1.0f), "-50 pts");
			score -= 50;
		}

		return true; //return true for collision responce or false for detecting object overlap only
	}

	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);

		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "     Score: %d", score);
	}

	virtual void OnCleanupScene() override
	{
		Scene::OnCleanupScene();
		s.clear();
	}

private:
	GameObject* obj;
	vector<GameObject*> s;
	int score;
};