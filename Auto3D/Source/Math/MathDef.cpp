#include "MathDef.h"

namespace Auto3D
{

void SinCos(float angle, float& sin, float& cos)
{
	float angleRadians = angle * M_DEGTORAD;
#if defined(HAVE_SINCOSF)
	sincosf(angleRadians, &sin, &cos);
#elif defined(HAVE___SINCOSF)
	__sincosf(angleRadians, &sin, &cos);
#else
	sin = sinf(angleRadians);
	cos = cosf(angleRadians);
#endif
}

}