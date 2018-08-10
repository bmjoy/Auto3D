#pragma once
#include "Auto.h"
#include "ComponentSetting.h"
#include "Shader.h"
namespace Auto3D {
/**
* @brief : Multisampled anti-aliasing, sampling point clamp (1~8)
*/
enum PostProcessingMode
{
	POST_DEFAULT,
	POST_BULR,
	POST_EDGE_DETECTION,
	POST_GRAYSCALE,
	POST_INVERSION,
	POST_SHARPEN,
};
class MSAA : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(MSAA, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(MSAA);
public:
	/**
	* @brief : Set sampling point count
	*/
	explicit MSAA(Ambient* ambient, int pointNum = 4);
	/**
	* @brief : Render before work
	*/
	void RenderStart();
	/**
	* @brief : Render after work
	*/
	void RenderEnd();


	void SetEffect(PostProcessingMode mode);
	void SetEffect(const Shader& shader);
	void Enable(bool enable) { _enable = enable; }
	bool GetEnable() { return _enable; }
private:
	bool _enable;
	Shader shader;
	Shader shaderBlur;
	Shader shaderEdgeDetection;
	Shader shaderGrayscale;
	Shader shaderInversion;
	Shader shaderSharpen;


private:
	Shader _shader;
	int _samplingPointCount;
	unsigned int _quadVAO, _quadVBO;
	unsigned int _framebuffer;
	unsigned int _textureColorBufferMultiSampled;
	unsigned int _rbo;
	unsigned int _screenTexture;
	unsigned int _intermediateFBO;
};

}
