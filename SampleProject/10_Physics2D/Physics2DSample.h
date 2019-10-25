#pragma once
#include "../Sample.h"

using namespace Auto3D;

class Physics2DSample : public Sample
{
	REGISTER_OBJECT_CLASS(Physics2DSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	float yaw = 0.0f, pitch = 20.0f;
	Camera2D* camera2d;
};