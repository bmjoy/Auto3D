#include "FreeCamera.h"
#include "BaseSpace.h"
#include "Time.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "Light.h"
#include "Input.h"


void FreeCamera::processInput()
{
	
	if (GetSubSystem<Input>()->GetKeyPress(KEY_W))
		freeCamera->ProcessKeyboard(FORWARD, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_S))
		freeCamera->ProcessKeyboard(BACKWARD, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_A))
		freeCamera->ProcessKeyboard(LEFT, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_D))
		freeCamera->ProcessKeyboard(RIGHT, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->IsMouseMove())
	{
		freeCamera->ProcessMouseMovement(GetSubSystem<Input>()->GetMouseMove().x, GetSubSystem<Input>()->GetMouseMove().y);
	}
	freeCamera->ProcessMouseScroll(GetSubSystem<Input>()->GetMouseWheelOffset());
}


FreeCamera::FreeCamera(Ambient* ambient)
	:ScriptComponent(ambient)
{
	freeCameraObject = new GameObject(_ambient);
	freeCamera = new Camera(_ambient);
	freeCamera->SetFar(1000.0f);
}
FreeCamera::~FreeCamera()
{
}

void FreeCamera::Start()
{
	
	
	freeCameraObject->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);

	freeCameraObject->AddComponent(freeCamera);

	GetSubSystem<Input>()->ShowCursor(false);
	GetSubSystem<Input>()->LockCursorInCenter();
	
}
void FreeCamera::Update()
{
	processInput();
	
}
