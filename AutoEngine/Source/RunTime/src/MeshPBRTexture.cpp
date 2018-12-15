#include "MeshPBRTexture.h"
#include "Configs.h"
#include "BaseMesh.h"
#include "HDRSkyBox.h"
#include "Renderer.h"
#include "Time.h"
#include "Transform.h"
#include "Light.h"
#include "ResourceSystem.h"
#include "NewDef.h"

namespace Auto3D {

MeshPBRTexture::MeshPBRTexture(Ambient* ambient)
	: Super(ambient)
	, _shaderTexture(shader_path + "au_pbr.auvs"
		, shader_path + "au_pbr_hdr_trxture.aufs")
	, _shaderNoTexture(shader_path + "au_pbr.auvs"
		, shader_path + "au_pbr_texture.aufs")
{
	
}


MeshPBRTexture::~MeshPBRTexture()
{
	UnloadOpaque(this);
}

void MeshPBRTexture::Start()
{
	if (SkyBoxManager::Instance().GetEnable())
	{
		_tshader = _shaderTexture;
	}
	else
	{
		_tshader = _shaderNoTexture;
	}
	_tshader.Use();

	_tshader.SetInt("albedoMap", 0);
	_tshader.SetInt("normalMap", 1);
	_tshader.SetInt("metallicMap", 2);
	_tshader.SetInt("roughnessMap", 3);
	_tshader.SetInt("aoMap", 4);
	
	if (SkyBoxManager::Instance().GetEnable())
	{
		
		_tshader.SetInt("irradianceMap", 5);
		_tshader.SetInt("prefilterMap", 6);
		_tshader.SetInt("brdfLUT", 7);
	}
	/*_albedoMap = LocalTextureLoad("../Resource/texture/pbr/gold/albedo.png");
	_normalMap = LocalTextureLoad("../Resource/texture/pbr/gold/normal.png");
	_metallicMap = LocalTextureLoad("../Resource/texture/pbr/gold/metallic.png");
	_roughnessMap = LocalTextureLoad("../Resource/texture/pbr/gold/roughness.png");
	_aoMap = LocalTextureLoad("../Resource/texture/pbr/gold/ao.png");*/

	_albedoMap = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/pbr/gold/albedo.png");
	_normalMap = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/pbr/gold/normal.png");
	_metallicMap = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/pbr/gold/metallic.png");
	_roughnessMap = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/pbr/gold/roughness.png");
	_aoMap = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/pbr/gold/ao.png");

	RegisterOpaque(this);
}

void MeshPBRTexture::Draw()
{
	//////////////////////////////////////////////////////////////////////////
	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();

	_tshader.Use();
	_tshader.SetMat4("projection", projection);
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	_tshader.SetMat4("view", view);
	_tshader.SetVec3("camPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _albedoMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _normalMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _metallicMap);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _roughnessMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _aoMap);
	// bind pre-computed IBL data
	if (SkyBoxManager::Instance().GetEnable())
	{
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxManager::Instance().GetSkyBox()->irradianceMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxManager::Instance().GetSkyBox()->prefilterMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, SkyBoxManager::Instance().GetSkyBox()->brdfLUTTexture);
		
	}
	else
	{
		
	}

	

	/////////////////////////////////////////////////////////////////////////////////////////////
	//light
	VECTOR<Light*> lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	int lightNum = 0;
	for (VECTOR<Light*>::iterator it = lights.begin(); it != lights.end(); it++)
	{
		_tshader.SetVec3("lightPositions[" + KhSTL::ToString(lightNum) + "]", (*it)->GetNode().GetPosition());
		_tshader.SetVec3("lightColors[" + KhSTL::ToString(lightNum) + "]", (*it)->GetColorToVec());
		lightNum++;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	//model
	glm::mat4 modelMat;

	if (GetNodePtr())		//if gameObject not empty
		modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	_tshader.SetMat4("model", modelMat);

	renderSphere(&_vao, &_indexCount);

}


}
