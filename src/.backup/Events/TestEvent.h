#pragma once
#include <Windows.h>
#include <Render.h>
#include <Engine.h>
#include <GameObject.h>

void RunEventTest();
namespace gce{
	class OnRotateEvent
	{
	public:
		GameObject* source;
		float deltaTime;
		OnRotateEvent(GameObject* obj, float dt) : source(obj), deltaTime(dt) {}
	};
	class RotationListener
	{
	public:
		void OnRotate(const OnRotateEvent& evt)
		{
			// Handle rotation event
			OutputDebugStringA(
				("OnRotateEvent: GameObject ID = " + std::to_string(evt.source->GetID()) +
					", DeltaTime = " + std::to_string(evt.deltaTime) + ", angle" + std::to_string(evt.source->transform.GetWorldRotation().GetY()) + "\n").c_str()
			);		
		}
	};

}