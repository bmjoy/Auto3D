#include "Debug/Log.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Resource/ResourceCache.h"
#include "Graphics/Texture.h"
#include "Engine/Components/Camera.h"
#include "Scene/GeometryNode.h"
#include "Engine/Components/Material.h"

#include "Core/Modules/ModuleManager.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{

Geometry::Geometry() : 
    _primitiveType(EPrimitiveType::TRIANGLE_LIST),
    _drawStart(0),
    _drawCount(0),
    _lodDistance(0.0f)
{
}

Geometry::~Geometry()
{
}

void Geometry::Draw(AGraphics* graphics)
{
    graphics->SetVertexBuffer(0, _vertexBuffer.Get());
    if (_indexBuffer.Get())
    {
        graphics->SetIndexBuffer(_indexBuffer.Get());
        graphics->DrawIndexed(_primitiveType, _drawStart,_drawCount, 0);
    }
    else
        graphics->Draw(_primitiveType, _drawStart, _drawCount);
}

void Geometry::DrawInstanced(AGraphics* graphics, size_t start, size_t count)
{
	graphics->SetVertexBuffer(0, _vertexBuffer.Get());
    if (_indexBuffer.Get())
    {
        graphics->SetIndexBuffer(_indexBuffer.Get());
        graphics->DrawIndexedInstanced(_primitiveType, _drawStart, _drawCount, 0, start, count);
    }
    else
        graphics->DrawInstanced(_primitiveType, _drawStart, _drawCount, start, count);
}

SourceBatch::SourceBatch()
{
}

SourceBatch::~SourceBatch()
{
}

AGeometryNode::AGeometryNode() :
    _lightList(nullptr),
    _geometryType(GeometryType::STATIC)
{
    SetFlag(NF_GEOMETRY, true);
}

AGeometryNode::~AGeometryNode()
{
}

void AGeometryNode::RegisterObject()
{
    RegisterFactory<AGeometryNode>();
    CopyBaseAttributes<AGeometryNode, AOctreeNode>();
    RegisterMixedRefAttribute("materials", &AGeometryNode::MaterialsAttr, &AGeometryNode::SetMaterialsAttr,
        FResourceRefList(AMaterial::GetTypeStatic()));
}

void AGeometryNode::OnPrepareRender(unsigned frameNumber, ACamera* camera)
{
    _lastFrameNumber = frameNumber;
    _lightList = nullptr;
    _distance = camera->Distance(GetWorldPosition());
}

void AGeometryNode::SetGeometryType(GeometryType::Type type)
{
    _geometryType = type;
}

void AGeometryNode::SetNumGeometries(size_t num)
{
    _batches.Resize(num);
    
    // Ensure non-null materials
    for (auto it = _batches.Begin(); it != _batches.End(); ++it)
    {
        if (!it->_material.Get())
            it->_material = AMaterial::DefaultMaterial();
    }
}

void AGeometryNode::SetGeometry(size_t index, Geometry* geometry)
{
    if (!geometry)
    {
        ErrorString("Can not assign null geometry");
        return;
    }

    if (index < _batches.Size())
        _batches[index]._geometry = geometry;
    else
        ErrorStringF("Out of bounds batch index %d for setting geometry", (int)index);
}

void AGeometryNode::SetMaterial(AMaterial* material)
{
    if (!material)
        material = AMaterial::DefaultMaterial();

    for (size_t i = 0; i < _batches.Size(); ++i)
        _batches[i]._material = material;
}

void AGeometryNode::SetMaterial(size_t index, AMaterial* material)
{
    if (index < _batches.Size())
    {
        if (!material)
            material = AMaterial::DefaultMaterial();
        _batches[index]._material = material;
    }
    else
        ErrorStringF("Out of bounds batch index %d for setting material", (int)index);
}

void AGeometryNode::SetLocalBoundingBox(const BoundingBoxF& box)
{
    _boundingBox = box;
    // Changing the bounding box may require octree reinsertion
    AOctreeNode::OnTransformChanged();
}

Geometry* AGeometryNode::GetGeometry(size_t index) const
{
    return index < _batches.Size() ? _batches[index]._geometry.Get() : nullptr;
}

AMaterial* AGeometryNode::GetMaterial(size_t index) const
{
    return index < _batches.Size() ? _batches[index]._material.Get() : nullptr;
}

void AGeometryNode::OnWorldBoundingBoxUpdate() const
{
    _worldBoundingBox = _boundingBox.Transformed(GetWorldTransform());
    SetFlag(NF_BOUNDING_BOX_DIRTY, false);
}

void AGeometryNode::SetMaterialsAttr(const FResourceRefList& materials)
{
	ResourceCache* cache = GModuleManager::Get().CacheModule();
    for (size_t i = 0; i < materials._names.Size(); ++i)
        SetMaterial(i, cache->LoadResource<AMaterial>(materials._names[i]));
}

FResourceRefList AGeometryNode::MaterialsAttr() const
{
    FResourceRefList ret(AMaterial::GetTypeStatic());
    
    ret._names.Resize(_batches.Size());
    for (size_t i = 0; i < _batches.Size(); ++i)
        ret._names[i] = ResourceName(_batches[i]._material.Get());

    return ret;
}

}