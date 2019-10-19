#pragma once

// Convenience header file for including commonly needed engine classes. Note: intentionally does not include Debug/DebugNew.h
// so that placement new works as expected.

#include "Container/HashSet.h"
#include "Container/List.h"
#include "Container/Ptr.h"
#include "Base/ProcessUtils.h"
#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderVariation.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexBuffer.h"
#include "IO/File.h"
#include "IO/FileSystem.h"
#include "IO/MemoryBuffer.h"
#include "IO/VectorBuffer.h"
#include "Math/Frustum.h"
#include "Math/Polyhedron.h"
#include "Math/Random.h"
#include "Math/Ray.h"
#include "Object/Serializable.h"
#include "Renderer/Camera.h"
#include "Renderer/Light.h"
#include "Renderer/Material.h"
#include "Renderer/Model.h"
#include "Renderer/Octree.h"
#include "Renderer/Renderer.h"
#include "Renderer/StaticModel.h"
#include "Renderer/DynamicModel.h"
#include "Resource/Image.h"
#include "Resource/JSONFile.h"
#include "Resource/ResourceCache.h"
#include "Renderer/SkyBox.h"
#include "Renderer/Water.h"
#include "Scene/Scene.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Thread/Condition.h"
#include "Thread/Mutex.h"
#include "Thread/Thread.h"
#include "Time/Time.h"
#include "Platform/Input.h"
#include "Platform/Window.h"
#include "Audio/Audio.h"
#include "Audio/Sound.h"
#include "Audio/AudioListener.h"
#include "Audio/AudioSource.h"
#include "Audio/AudioNode.h"
#include "Script/Script.h"
#include "Auto2D/Renderer2D.h"
#include "Auto2D/Batch2D.h"
#include "Auto2D/Camera2D.h"
#include "Auto2D/Scene2D.h"
#include "Auto2D/Sprite2D.h"
#include "Auto2D/PhysicsWorld2D.h"
#include "Auto2D/RigidBody2D.h"
#include "Auto2D/Collider2D.h"
#include "Auto2D/ColliderBox2D.h"
#include "UI/UI.h"
#include "UI/Canvas.h"
#include "UI/Text.h"
#include "UI/Button.h"
#include "UI/Slider.h"
#include "UI/ColorEdit.h"
#include "UI/CheckBox.h"
#include "Physics/Physics.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsUtils.h"
#include "Physics/RigidBody.h"
#include "Physics/Collider.h"
#include "Physics/ColliderBox.h"
#include "Physics/ColliderSphere.h"
#include "Engine/ModuleManager.h"

#include "Adapter/AutoFileDialog.h"

#include "Application.h"