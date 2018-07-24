#include "AntiAliasingSpace.h"
#include "DebugSapce.h"
#include "DeferredShadingSpace.h"
#include "DepthTestSpace.h"
#include "FrameBuffersSpace.h"
#include "BloomSpace.h"
#include "HDRSpace.h"
#include "MassInstantiationSpace.h"
#include "MoreCameraSpace.h"
#include "MultLightsSpace.h"
#include "PBRSpace.h"
#include "PBRSkyBoxSpace.h"
#include "ShadowSpace.h"
#include "SkyBoxSpace.h"
#include "SSAOSpace.h"
#include "StencilTestSpace.h"
#include "TextureSpace.h"
#include "NormalMappingSpace.h"
#include "ParallaxMappingSpace.h"
#include "VertexExplodeSpace.h"
#include "Ambient.h"
int main(int argc, char** argv)
{
	Ambient* ambient = new Ambient();


	//AntiAliasingSpace work;
	//DebugSapce work;
	//DeferredShadingSpace work;
	//DepthTestSpace work;
	//FrameBuffersSpace work;
	//BloomSpace work;
	//HDRSpace work;
	//MassInstantiationSpace work;
	//MoreCameraSpace work;
	//MultLightsSpace work;
	//PBRSpace work;
	//PBRSkyBoxSpace work;
	//ShadowSpace work;
	//SkyBoxSpace work;
	//SSAOSpace work;
	//StencilTestSpace work;
	TextureSpace work(ambient);
	//NormalMappingSpace work;
	//ParallaxMappingSpace work;
	//VertexExplodeSpace work;
	return work.Launch();
}
