#include "Mesh.h"
#include "RenderManager.h"
AUTO_BEGIN

Mesh::Mesh()
	: m_meshPath("Resource/object/nanosuit/nanosuit.obj")
	, m_shader(Shader(AtConfig::shader_path + "au_model_loading.auvs", AtConfig::shader_path + "au_model_loading.aufs"))
{
}

Mesh::Mesh(_String meshPath)
	: m_meshPath(meshPath)
	, m_shader(Shader(AtConfig::shader_path + "au_text_model_loading.auvs", AtConfig::shader_path + "au_text_model_loading.aufs"))
{
}
Mesh::Mesh(_String meshPath, const Shader& shader)
	: m_meshPath(meshPath)
	, m_shader(shader)
{
}
Mesh::~Mesh()
{
}
void Mesh::Start()
{
	m_Model = LocalModelLoad((char*)m_meshPath.data());
}
void Mesh::Update(Camera * cam)
{
	if (cam == nullptr)
	{
		WarningString("Fail to find camera");
		return;
	}
	m_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	//Camera * cam;
	//if(INSTANCE(RenderManager).CameraArray.find(0)->second)
	//	cam = INSTANCE(RenderManager).CameraArray.find(0)->second;
	//else
	//{
	//	ErrorString("Fail to find Camera.");
	//	return;
	//}
	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = cam->GetViewMatrix();
	projectionMat = glm::perspective(cam->Zoom, (float)cam->GetScreenRect().width / (float)cam->GetScreenRect().height, cam->Near, cam->Far);
	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);

	m_shader.SetVec3("ourColor", m_Color.r, m_Color.g, m_Color.b);
	m_shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	m_shader.SetVec3("lightPos", 2.5f, 2.0f, 1.0f);
	m_shader.SetVec3("viewPos", INSTANCE(RenderManager).CameraArray.find(0)->second->Position);
	m_Model.Draw(m_shader);
}
void Mesh::SetColor(const Color& color)
{
	m_Color.Set(color.r, color.g, color.b, color.a);
}

void Mesh::SetColor(const Vector3& vec)
{
	m_Color.Set(vec.x, vec.y, vec.z, 1.0f);
}
void Mesh::SetColor(float r, float g, float b, float a)
{
	m_Color.Set(r, g, b, a);
}

AUTO_END