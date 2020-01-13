#include "Pawn.h"
#include "GameFramework/Controller.h"

namespace Auto3D
{

void APawn::PossessedBy(AController* newController)
{
	const AController*  oldController = _controller;

	_controller = newController;

	_isPossess = true;
}

void APawn::UnPossessed()
{
	const AController* oldController = _controller;

	_controller = nullptr;
	_isPossess = false;
}

AController* APawn::GetController() const
{
	return _controller;
}

}