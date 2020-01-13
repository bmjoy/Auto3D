#include "Adapter/AutoRttr.h"

#include "_Application.h"
#include "Thread/Thread.h"
#include "Core/ProcessUtils.h"
#include "Engine/Engine.h"
#include "Core/ClassRegister.h"

#include "Debug/DebugNew.h"


namespace Auto3D
{

AApplication::AApplication() :
	_exitCode(EXIT_SUCCESS)
{
	if (!_engine)
		_engine = MakeUnique<AEngine>();
}

AApplication::~AApplication()
{
}

int AApplication::Run()
{
	// Make sure the engine is created properly
	if (_engine.Null())
		_engine = MakeUnique<AEngine>();
	// Class register.
	FClassRegister classRegister;

#if !defined(__GNUC__) || __EXCEPTIONS
	try
	{
#endif
		Init();
		if (!_engine->Init())
		{
			ErrorExit();
			return _exitCode;
		}

		Start();
		if (_exitCode)
			return _exitCode;

		while (!_engine->IsExiting())
		{
			if (_engine->Update())
			{
				Update();
				_engine->Render();
				UIDraw();
				_engine->FrameFinish();
			}
			else
				// If winodw is minimized reduce CPU load keep FPS to a minimum
				FThread::Sleep(1000 / MIN_FRAMES_PER_SECOND);
		}
		Stop();
		_engine->Exit();

		return _exitCode;
#if !defined(__GNUC__) || __EXCEPTIONS
	}
	catch (std::bad_alloc&)
	{
		ErrorDialog(GetTypeName(), "An application that has an out-of-memory condition will exit immediately.");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
#endif
}

void AApplication::ErrorExit(const FString& message)
{
	// Close the rendering window
	_engine->Exit(); 
	_exitCode = EXIT_FAILURE;

	if (!message.Length())
	{
		ErrorDialog(GetTypeName(), _startupErrors.Length() ? _startupErrors :
			"Application has been terminated due to unexpected error.");
	}
	else
		ErrorDialog(GetTypeName(), message);
}

}