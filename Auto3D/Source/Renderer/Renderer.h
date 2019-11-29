#pragma once

#include "Container/AutoPtr.h"
#include "Graphics/Texture.h"
#include "Math/Color.h"
#include "Math/Frustum.h"
#include "Engine/Components/Image.h"
#include "Renderer/RendererInterface.h"

#include "RenderPath.h"
#include "Batch.h"

namespace Auto3D
{

class ConstantBuffer;
class GeometryNode;
class Octree;
class Scene;
class VertexBuffer;

/// Shader constant buffers used by high-level rendering.
namespace RendererConstantBuffer
{
	enum Type
	{
		FRAME = 0,
		OBJECT,
		LIGHTS,
		MATERIAL
	};
};


/// Parameter indices in constant buffers used by high-level rendering.
static const size_t VS_FRAME_VIEW_MATRIX = 0;
static const size_t VS_FRAME_PROJECTION_MATRIX = 1;
static const size_t VS_FRAME_VIEWPROJ_MATRIX = 2;
static const size_t VS_FRAME_DEPTH_PARAMETERS = 3;
static const size_t VS_OBJECT_WORLD_MATRIX = 0;
static const size_t VS_LIGHT_SHADOW_MATRICES = 0;
static const size_t PS_FRAME_AMBIENT_COLOR = 0;
static const size_t PS_FRAME_VIEW_POSITION = 1;
static const size_t PS_LIGHT_POSITIONS = 0;
static const size_t PS_LIGHT_DIRECTIONS = 1;
static const size_t PS_LIGHT_ATTENUATIONS = 2;
static const size_t PS_LIGHT_COLORS = 3;
static const size_t PS_LIGHT_SHADOW_PARAMETERS = 4;
static const size_t PS_LIGHT_DIR_SHADOW_SPLITS = 5;
static const size_t PS_LIGHT_DIR_SHADOW_FADE = 6;
static const size_t PS_LIGHT_POINT_SHADOW_PARAMETERS = 7;

/// Texture coordinate index for the instance world matrix.
static const size_t INSTANCE_TEXCOORD = 4;


/// High-level rendering subsystem. Performs rendering of 3D scenes.
class AUTO_API Renderer : public ABaseModule
{
    REGISTER_OBJECT_CLASS(Renderer , ABaseModule)

public:
    /// Construct and register subsystem.
    Renderer();
    /// Destruct.
    ~Renderer();

	/// Render scene
	void Render(Scene* scene, Camera* camera);
    /// Set number, size and format of shadow maps. These will be divided among the lights that need to render shadow maps.
    void SetupShadowMaps(size_t num, int size, ImageFormat::Type format);
	/// Prepare a view for rendering. Convenience function that calls CollectObjects(), CollectLightInteractions() and CollectBatches() in one go. Return true on success.
    bool PrepareView(Scene* scene, Camera* camera, const TVector<RenderPassDesc>& passes);
    /// Initialize rendering of a new view and collect visible objects from the camera's point of view. Return true on success (scene, camera and octree are non-null.)
    bool CollectObjects(Scene* scene, Camera* camera);
    /// Collect light interactions with geometries from the current view. If lights are shadowed, collects batches for shadow casters.
    void CollectLightInteractions();
    /// Collect and sort batches from the visible objects. To not go through the objects several times, all the passes should be specified at once instead of multiple calls to CollectBatches().
    void CollectBatches(const TVector<RenderPassDesc>& passes);
    /// Collect and sort batches from the visible objects. Convenience function for one pass only.
    void CollectBatches(const RenderPassDesc& pass);
	/// Render shadow maps. Should be called after all CollectBatches() calls but before RenderBatches(). Note that you must reassign your rendertarget and viewport after calling this.
    void RenderShadowMaps();
    /// Render several passes to the currently set rendertarget and viewport. Avoids setting the per-frame constants multiple times.
    void RenderBatches(const TVector<RenderPassDesc>& passes);
    /// Render a pass to the currently set rendertarget and viewport. Convenience function for one pass only.
    void RenderBatches(const FString& pass);

    /// Per-frame vertex shader constant buffer.
    TSharedPtr<ConstantBuffer> _vsFrameConstantBuffer;
    /// Per-frame pixel shader constant buffer.
    TSharedPtr<ConstantBuffer> _psFrameConstantBuffer;
    /// Per-object vertex shader constant buffer.
    TSharedPtr<ConstantBuffer> _vsObjectConstantBuffer;
    /// Lights vertex shader constant buffer.
    TSharedPtr<ConstantBuffer> _vsLightConstantBuffer;
    /// Lights pixel shader constant buffer.
    TSharedPtr<ConstantBuffer> _psLightConstantBuffer;

	/// Start up event;
	FEvent _startUpEvent;
private:
    /// Initialize. Needs the Graphics subsystem and rendering context to exist.
    void Initialize();
    /// (Re)define face selection textures.
    void DefineFaceSelectionTextures();
    /// Octree callback for collecting lights and geometries.
    void CollectGeometriesAndLights(TVector<OctreeNode*>::ConstIterator begin, TVector<OctreeNode*>::ConstIterator end, bool inside);
    /// Assign a light list to a node. Creates new light lists as necessary to _handle multiple lights.
    void AddLightToNode(GeometryNode* node, Light* light, LightList* lightList);
    /// Collect shadow caster batches.
    void CollectShadowBatches(const TVector<GeometryNode*>& nodes, RenderQueue& batchQueue, const Frustum& frustum, bool checkShadowCaster, bool checkFrustum);
    /// Render batches from a specific queue and camera.
    void RenderBatches(const TVector<Batch>& batches, Camera* camera, bool setPerFrameContants = true, bool overrideDepthBias = false, int depthBias = 0, float slopeScaledDepthBias = 0.0f);
    /// Load shaders for a pass.
    void LoadPassShaders(Pass* pass);
    /// Return or create a shader variation for a pass. Vertex shader variations _handle different geometry types and pixel shader variations _handle different light combinations.
    ShaderVariation* FindShaderVariation(ShaderStage::Type stage, Pass* pass, unsigned short bits);
    
    /// Graphics subsystem pointer.
    TWeakPtr<Graphics> _graphics;
    /// Current scene.
    Scene* _scenes;
    /// Current scene camera.
    Camera* _camera;
    /// Current octree.
    Octree* _octree;
    /// Camera's view frustum.
    Frustum _frustum;
    /// Camera's view mask.
    unsigned _viewLayoutMask;
    /// Geometries in frustum.
    TVector<GeometryNode*> _geometries;
    /// Lights in frustum.
    TVector<Light*> _lights;
    /// Batch queues per pass.
    THashMap<unsigned char, RenderQueue> _batchQueues;
    /// Instance transforms for uploading to the instance vertex buffer.
    TVector<Matrix3x4F> _instanceTransforms;
    /// Lit geometries query result.
    TVector<GeometryNode*> _litGeometries;
    /// %Light lists.
    THashMap<unsigned long long, LightList> _lightLists;
    /// %Light passes.
    THashMap<unsigned long long, LightPass> _lightPasses;
    /// Ambient only light pass.
    LightPass _ambientLightPass;
    /// Current frame number.
    unsigned _frameNumber;
    /// Instance vertex buffer dirty flag.
    bool _instanceTransformsDirty;
    /// Shadow maps.
    TVector<ShadowMap> _shadowMaps;
    /// Shadow views.
    TVector<TAutoPtr<ShadowView> > _shadowViews;
    /// Used shadow views so far.
    size_t _usedShadowViews;
    /// Instance transform vertex buffer.
    TAutoPtr<VertexBuffer> _instanceVertexBuffer;
    /// Vertex elements for the instance vertex buffer.
    TVector<VertexElement> _instanceVertexElements;
    /// First point light face selection cube map.
    TAutoPtr<Texture> _faceSelectionTexture1;
    /// Second point light face selection cube map.
    TAutoPtr<Texture> _faceSelectionTexture2;
};

/// Register Renderer related object factories and attributes.
AUTO_API void RegisterRendererLibrary();

}

