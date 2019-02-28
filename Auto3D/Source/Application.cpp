#include "Application.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{

Application::Application()
{
	_engine = new Engine();
}


Application::~Application()
{
}

int Application::Run()
{
	try
	{
		Init();
		_engine->Init();

		Start();
		while (!_engine->IsExiting())
		{
			if (_engine->Update())
			{
				Update();
				_engine->Render();
				_engine->FrameFinish();
			}
		}
		Stop();
		_engine->Exit();

	}
	catch (std::bad_alloc&)
	{
		ErrorString("An application that has an out-of-memory condition will exit immediately.");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

}
