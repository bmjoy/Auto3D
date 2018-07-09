#include "ShadowSpace.h"
#include "Application.h"
#include "Light.h"
#include "FreeCamera.h"
#include "Mesh.h"
#include "Shadow.h"
ShadowSpace::ShadowSpace()
{}
ShadowSpace::~ShadowSpace()
{}

void ShadowSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * lightObj = new GameObject();
	Light * light = new Light(Directional);
	lightObj->AddComponent(light);

/*
	GameObject * meshObj = new GameObject();
	Mesh * mesh = new Mesh("Resource/object/base/Cube.FBX");
	meshObj->AddComponent(mesh);
	meshObj->GetComponent(Transform).SetPosition(0.0f, 1.0f, 0.0f);
	
	GameObject * meshObj2 = new GameObject();
	Mesh * mesh2 = new Mesh("Resource/object/base/Cube.FBX");
	meshObj2->AddComponent(mesh2);
	meshObj2->GetComponent(Transform).SetScale(10.0f, 0.5f, 10.0f);*/

	GameObject * shadowObj = new GameObject();
	Shadow * shadow = new Shadow();
	shadowObj->AddComponent(shadow);
}

void ShadowSpace::Update()
{
}

int ShadowSpace::Launch()
{
	return INSTANCE(Application).Run();
}
