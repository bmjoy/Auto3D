#pragma once
#include "../Sample.h"
using namespace Auto3D;

class TestSample : public Application
{
	REGISTER_OBJECT_CLASS(TestSample, Application)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;

	float yaw = 0.0f, pitch = 20.0f;
	Camera* camera;
	SharedPtr<Scene> scene;

	/// ShaderVariation vs 
	ShaderVariation* _vsv;
	/// ShaderVariation ps
	ShaderVariation* _psv;


	/// Per-frame vertex shader constant buffer.
	SharedPtr<ConstantBuffer> _vsFrameConstantBuffer;
	/// Per-object vertex shader constant buffer.
	SharedPtr<ConstantBuffer> _psFrameConstantBuffer;

	AutoPtr<SkyBoxBuffer> skyBoxBuffer;
	AutoPtr<Texture> textureCube;
};