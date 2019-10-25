#include "Physics2DSample.h"
void Physics2DSample::Init()
{
	Super::Init();
	auto* graphics = Object::Module<Graphics>();
	graphics->RenderWindow()->SetTitle("Physics2D Sample");
}

void Physics2DSample::Start()
{
	Super::Start();
	
	auto* cache = Object::Module<ResourceCache>();
	auto squareTexture = cache->LoadResource<Texture>("Texture/Box.png");
	auto circleTexture = cache->LoadResource<Texture>("Texture/Ball.png");

	PhysicsWorld2D* physicsWorld2d = scene2d->CreateChild<PhysicsWorld2D>();

	camera2d = scene2d->CreateChild<Camera2D>();
	camera2d->SetOrthoSize(50);
	camera2d->SetOrthographic(true);
	camera2d->SetPosition(Vector3F(0.0f, 0.0f, -100.0f));
	camera2d->SetLayoutMaskName("UI");

	{
		Sprite2D* plane = scene2d->CreateChild<Sprite2D>();
		plane->SetTexture(squareTexture);
		plane->SetPosition(Vector2F(0.0f, -24.0f));
		plane->SetScale(Vector3F(24.0f, 1.0f));
		RigidBody2D* planeRigidBody = plane->CreateChild<RigidBody2D>();
		planeRigidBody->SetBodyType(BodyType2D::STATIC);
		ColliderBox2D* planeColliderBox = plane->CreateChild<ColliderBox2D>();
		planeColliderBox->SetSize(Vector2F(24.0f, 1.0f));
		planeColliderBox->SetFriction(0.5f);

		//Wall right
		Sprite2D* wallRight = scene2d->CreateChild<Sprite2D>();
		wallRight->SetTexture(squareTexture);
		wallRight->SetPosition(Vector2F(23.0f, 0.0f));
		wallRight->SetScale(Vector3F(1.0f, 25.0f));
		RigidBody2D* wallRightRigidBody = wallRight->CreateChild<RigidBody2D>();
		wallRightRigidBody->SetBodyType(BodyType2D::STATIC);
		ColliderBox2D* wallRightColliderBox = wallRight->CreateChild<ColliderBox2D>();
		wallRightColliderBox->SetSize(Vector2F(1.0f, 25.0f));
		wallRightColliderBox->SetFriction(0.5f);

		//Wall left
		Sprite2D* wallLeft = scene2d->CreateChild<Sprite2D>();
		wallLeft->SetTexture(squareTexture);
		wallLeft->SetPosition(Vector2F(-24.0f,0.0f));
		wallLeft->SetScale(Vector3F(1.0f, 25.0f));
		RigidBody2D* wallLeftRigidBody = wallLeft->CreateChild<RigidBody2D>();
		wallLeftRigidBody->SetBodyType(BodyType2D::STATIC);
		ColliderBox2D* wallLeftColliderBox = wallLeft->CreateChild<ColliderBox2D>();
		wallLeftColliderBox->SetSize(Vector2F(1.0f, 25.0f));
		wallLeftColliderBox->SetFriction(0.5f);
	}
	

	int layerCount = 10;
	for (int i = 0; i < layerCount; ++i)
	{
		if (i % 2 == 0)
		{
			static int count = layerCount / 2 + layerCount % 2;
			const int countI = layerCount / 2 + layerCount % 2;
			for (int k = 0; k <= countI - count; ++k)
			{
				Sprite2D* node = scene2d->CreateChild<Sprite2D>();
				node->SetPosition(Vector2F(0.0f + k * 3.0f, 20.0f + i * 3.0f));
				node->SetScale(Vector3F(1.0f, 1.0f));
				node->SetTexture(squareTexture);
				RigidBody2D* nodeRigidBody = node->CreateChild<RigidBody2D>();
				nodeRigidBody->SetBodyType(BodyType2D::DYNAMIC);

				ColliderBox2D* squareColliderBox = node->CreateChild<ColliderBox2D>();
				squareColliderBox->SetSize(Vector2F(1.0f, 1.0f));
				squareColliderBox->SetDensity(1.0f);
				squareColliderBox->SetFriction(0.5f);
				squareColliderBox->SetRestitution(0.1f);
			}

			for (int k = 0; k <= countI - count; ++k)
			{
				Sprite2D* node = scene2d->CreateChild<Sprite2D>();
				node->SetPosition(Vector2F(0.0f - k * 3.0f, 20.0f + i * 3.0f));
				node->SetScale(Vector3F(1.0f, 1.0f));
				node->SetTexture(squareTexture);
				RigidBody2D* nodeRigidBody = node->CreateChild<RigidBody2D>();
				nodeRigidBody->SetBodyType(BodyType2D::DYNAMIC);

				ColliderBox2D* squareColliderBox = node->CreateChild<ColliderBox2D>();
				squareColliderBox->SetSize(Vector2F(1.0f, 1.0f));
				squareColliderBox->SetDensity(1.0f);
				squareColliderBox->SetFriction(0.5f);
				squareColliderBox->SetRestitution(0.1f);
			}
			count--;
		}
		else
		{
			static int count = layerCount / 2;
			const int countI = layerCount / 2;
			for (int k = 0; k <= countI - count; ++k)
			{
				Sprite2D* node = scene2d->CreateChild<Sprite2D>();
				node->SetPosition(Vector2F(0.0f + k * 3.0f, 20.0f + i * 3.0f));
				node->SetScale(Vector3F(1.0f, 1.0f));
				node->SetTexture(circleTexture);
				RigidBody2D* nodeRigidBody = node->CreateChild<RigidBody2D>();
				nodeRigidBody->SetBodyType(BodyType2D::DYNAMIC);

				
				ColliderCircle2D* squareColliderBox = node->CreateChild<ColliderCircle2D>();
				squareColliderBox->SetRadius(1.0f);
				squareColliderBox->SetDensity(1.0f);
				squareColliderBox->SetFriction(0.5f);
				squareColliderBox->SetRestitution(0.1f);
			}
			for (int k = 0; k <= countI - count; ++k)
			{
				Sprite2D* node = scene2d->CreateChild<Sprite2D>();
				node->SetPosition(Vector2F(0.0f - k * 3.0f, 20.0f + i * 3.0f));
				node->SetScale(Vector3F(1.0f, 1.0f));
				node->SetTexture(circleTexture);
				RigidBody2D* nodeRigidBody = node->CreateChild<RigidBody2D>();
				nodeRigidBody->SetBodyType(BodyType2D::DYNAMIC);

				ColliderCircle2D* squareColliderBox = node->CreateChild<ColliderCircle2D>();
				squareColliderBox->SetRadius(1.0f);
				squareColliderBox->SetDensity(1.0f);
				squareColliderBox->SetFriction(0.5f);
				squareColliderBox->SetRestitution(0.1f);
			}
			count--;
		}
	}
}
void Physics2DSample::Update()
{
	Super::Update();

	auto input = ModuleManager::Get().InputModule();
	auto renderer = ModuleManager::Get().RendererModule();
	auto time = ModuleManager::Get().TimeModule();

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	if (input->IsKeyDown(KEY_W))
		camera2d->Translate(Vector3F::UP * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera2d->Translate(Vector3F::DOWN * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera2d->Translate(Vector3F::LEFT * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera2d->Translate(Vector3F::RIGHT * time->GetDeltaTime() * moveSpeed);
}

void Physics2DSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(Physics2DSample)