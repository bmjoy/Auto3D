#pragma once

#include "../Sample.h"

using namespace Auto3D;

class FirstPersonSample : public Sample
{
	DECLARE_CLASS(FirstPersonSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;

	float yaw = 0.0f, pitch = 0.0f;
	ACamera* camera;
	TSharedPtr<AWorld> scene;

	typedef struct RandMSG
	{
		ALight* light;
		TVector3F position;
		TVector3F color;
		float xRand;
		float yRand;
		float zRand;
	};

	TVector<RandMSG> lights;
};