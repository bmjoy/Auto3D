#include "Debug/Log.h"
#include "IO/Stream.h"
#include "Core/ObjectResolver.h"
#include "Resource/JSONFile.h"
#include "Engine/Components/Camera.h"
#include "Scene.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

REGISTER_CLASS
{
	using namespace rttr;
	FRegistration::class_<ANode>("Node")
	.constructor<>()
		.property("name", &ANode::GetName, &ANode::SetName)
		.property("enabled", &ANode::IsEnabled, &ANode::SetEnabled)
		.property("temporary", &ANode::IsTemporary, &ANode::SetTemporary)
		.property("layer", &ANode::GetLayer, &ANode::SetLayer)
		.property("tag", &ANode::GetTag, &ANode::SetTag)
		;
}

static TVector<TSharedPtr<ANode> > noChildren;

ANode::ANode() :
    _flags(NF_ENABLED),
    _layer(LAYER_DEFAULT),
    _tag(TAG_NONE),
    _parent(nullptr),
    _scenes(nullptr),
    _id(0)
{
}

ANode::~ANode()
{
    RemoveAllChildren();
    // At the time of destruction the node should not have a parent, or be in a scene
    assert(!_parent);
    assert(!_scenes);
}

void ANode::RegisterObject()
{
    RegisterFactory<ANode>();
    RegisterRefAttribute("name", &ANode::GetName, &ANode::SetName);
    RegisterAttribute("enabled", &ANode::IsEnabled, &ANode::SetEnabled, true);
    RegisterAttribute("temporary", &ANode::IsTemporary, &ANode::SetTemporary, false);
    RegisterAttribute("layer", &ANode::GetLayer, &ANode::SetLayer, LAYER_DEFAULT);
    RegisterAttribute("tag", &ANode::GetTag, &ANode::SetTag, TAG_NONE);
}

void ANode::Load(FStream& source, FObjectResolver& resolver)
{
    // Type and _id has been read by the parent
    ASerializable::Load(source, resolver);

    size_t numChildren = source.ReadVLE();
    for (size_t i = 0; i < numChildren; ++i)
    {
        FStringHash childType(source.Read<FStringHash>());
        unsigned childId = source.Read<unsigned>();
        ANode* child = CreateChild(childType);
        if (child)
        {
            resolver.StoreObject(childId, child);
            child->Load(source, resolver);
        }
        else
        {
            // If child is unknown type, skip all its attributes and children
            SkipHierarchy(source);
        }
    }
}

void ANode::Save(FStream& dest)
{
    // Write type and ID first, followed by attributes and child nodes
    dest.Write(GetType());
    dest.Write(Id());
    ASerializable::Save(dest);
    dest.WriteVLE(NumPersistentChildren());

    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (!child->IsTemporary())
            child->Save(dest);
    }
}

void ANode::LoadJSON(const FJSONValue& source, FObjectResolver& resolver)
{
    // Type and _id has been read by the parent
    ASerializable::LoadJSON(source, resolver);
    
    const JSONArray& children = source["children"].GetArray();
    if (children.Size())
    {
        for (auto it = children.Begin(); it != children.End(); ++it)
        {
            const FJSONValue& childJSON = *it;
            FStringHash childType(childJSON["type"].GetString());
            unsigned childId = (unsigned)childJSON["id"].GetNumber();
            ANode* child = CreateChild(childType);
            if (child)
            {
                resolver.StoreObject(childId, child);
                child->LoadJSON(childJSON, resolver);
            }
        }
    }
}

void ANode::SaveJSON(FJSONValue& dest)
{
    dest["type"] = GetTypeName();
    dest["id"] = Id();
    ASerializable::SaveJSON(dest);
    
    if (NumPersistentChildren())
    {
        dest["children"].SetEmptyArray();
        for (auto it = _children.Begin(); it != _children.End(); ++it)
        {
            ANode* child = *it;
            if (!child->IsTemporary())
            {
                FJSONValue childJSON;
                child->SaveJSON(childJSON);
                dest["children"].Push(childJSON);
            }
        }
    }
}

void ANode::_SaveJSON(FJSONValue& dest)
{
	dest["type"] = GetTypeName();
	dest["id"] = Id();
	dest["children"].SetEmptyArray();
	FJSONValue childJSON;
	childJSON["a1"] = 1;
	childJSON["a2"] = "asd";
	childJSON["a3"] = 1.0f;
	dest["children"].Push(childJSON);

	FJSONValue arrayJSON;
	arrayJSON.SetEmptyArray();
	TVector<FString> arrayVec;
	arrayVec.Push("aaa");
	arrayVec.Push("bbb");
	for (auto it = arrayVec.Begin(); it != arrayVec.End(); ++it)
		arrayJSON.Push(*it);
	dest["arrayTest2"] = arrayJSON;

	//FResourceRef reourceRef = FResourceRef(AModel::GetTypeStatic(), ResourceName(_model.Get()));
	FResourceRef reourceRef = FResourceRef(FStringHash("AStaticModel"), "Box/Box.mdl");
	dest["model"] = reourceRef.ToString();

	//attr->ToJSON(this, dest[attr->GetName()]);->
	/*void ToJSON(EAttributeType::Type type, FJSONValue& dest, const void* source)
	{
		switch (type)
		{
		case EAttributeType::BOOL:
			dest = *(reinterpret_cast<const bool*>(source));
			break;

		case EAttributeType::BYTE:
			dest = *(reinterpret_cast<const unsigned char*>(source));
			break;

		case EAttributeType::UNSIGNED:
			dest = *(reinterpret_cast<const unsigned*>(source));
			break;

		case EAttributeType::INT:
			dest = *(reinterpret_cast<const int*>(source));
			break;

		case EAttributeType::INTVECTOR2:
			dest = reinterpret_cast<const TVector2I*>(source)->ToString();
			break;

		case EAttributeType::INTRECT:
			dest = reinterpret_cast<const TRectI*>(source)->ToString();
			break;

		case EAttributeType::FLOAT:
			dest = *(reinterpret_cast<const float*>(source));
			break;

		case EAttributeType::VECTOR2:
			dest = reinterpret_cast<const TVector2<float>*>(source)->ToString();
			break;

		case EAttributeType::VECTOR3:
			dest = reinterpret_cast<const TVector3F*>(source)->ToString();
			break;

		case EAttributeType::VECTOR4:
			dest = reinterpret_cast<const TVector4F*>(source)->ToString();
			break;

		case EAttributeType::QUATERNION:
			dest = reinterpret_cast<const FQuaternion*>(source)->ToString();
			break;

		case EAttributeType::COLOR:
			dest = reinterpret_cast<const FColor*>(source)->ToString();
			break;

		case EAttributeType::RECT:
			dest = reinterpret_cast<const TRectF*>(source)->ToString();
			break;

		case EAttributeType::BOUNDINGBOX:
			dest = reinterpret_cast<const TBoundingBoxF*>(source)->ToString();
			break;

		case EAttributeType::MATRIX3:
			dest = reinterpret_cast<const TMatrix3x3F*>(source)->ToString();
			break;

		case EAttributeType::MATRIX3X4:
			dest = reinterpret_cast<const TMatrix3x4F*>(source)->ToString();
			break;

		case EAttributeType::MATRIX4:
			dest = reinterpret_cast<const TMatrix4x4F*>(source)->ToString();
			break;

		case EAttributeType::STRING:
			dest = *(reinterpret_cast<const FString*>(source));
			break;

		case EAttributeType::RESOURCEREF:
			dest = reinterpret_cast<const FResourceRef*>(source)->ToString();
			break;

		case EAttributeType::RESOURCEREFLIST:
			dest = reinterpret_cast<const FResourceRefList*>(source)->ToString();
			break;

		case EAttributeType::OBJECTREF:
			dest = reinterpret_cast<const FObjectRef*>(source)->_id;
			break;

		case EAttributeType::JSONVALUE:
			dest = *(reinterpret_cast<const FJSONValue*>(source));
			break;

		default:
			break;
		}
	}*/
}

bool ANode::SaveJSON(FStream& dest)
{
    AJSONFile json;
    SaveJSON(json.Root());
    return json.Save(dest);
}

void ANode::SetName(const FString& newName)
{
    SetName(newName.CString());
}

void ANode::SetLayer(unsigned char newLayer)
{
    if (_layer < 32)
        _layer = newLayer;
    else
        ErrorString("Can not set layer 32 or higher");
}

void ANode::SetLayerName(const FString& newLayerName)
{
    if (!_scenes)
        return;
    
    const THashMap<FString, unsigned char>& layers = _scenes->Layers();
    auto it = layers.Find(newLayerName);
    if (it != layers.End())
		SetLayer(it->_second);
    else
        ErrorString("Layer " + newLayerName + " not defined in the scene");
}

void ANode::SetTag(unsigned char newTag)
{
    _tag = newTag;
}

void ANode::SetTagName(const FString& newTagName)
{
    if (!_scenes)
        return;

    const THashMap<FString, unsigned char>& tags = _scenes->Tags();
    auto it = tags.Find(newTagName);
    if (it != tags.End())
        SetTag(it->_second);
    else
        ErrorString("Tag " + newTagName + " not defined in the scene");
}

void ANode::SetEnabled(bool enable)
{
    SetFlag(NF_ENABLED, enable);
    OnSetEnabled(TestFlag(NF_ENABLED));
}

void ANode::SetEnabledRecursive(bool enable)
{
    SetEnabled(enable);
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        child->SetEnabledRecursive(enable);
    }
}

void ANode::SetTemporary(bool enable)
{
    SetFlag(NF_TEMPORARY, enable);
}

void ANode::SetParent(ANode* newParent)
{
    if (newParent)
        newParent->AddChild(this);
    else
        ErrorString("Could not set null parent");
}

void ANode::DefineLayer(unsigned char index, const FString& name)
{
	if (index >= 32)
	{
		ErrorString("Can not define more than 32 layers");
		return;
	}

	if (_layerNames.Size() <= index)
		_layerNames.Resize(index + 1);
	_layerNames[index] = name;
	_layers[name] = index;
}

void ANode::DefineTag(unsigned char index, const FString& name)
{
	if (_tagNames.Size() <= index)
		_tagNames.Resize(index + 1);
	_tagNames[index] = name;
	_tags[name] = index;
}

ANode* ANode::CreateChild(FStringHash childType)
{
    TSharedPtr<AObject> newObject = Create(childType);
    if (!newObject)
    {
        ErrorString("Could not create child node of unknown type " + childType.ToString());
        return nullptr;
    }
    ANode* child = dynamic_cast<ANode*>(newObject.Get());
    if (!child)
    {
        ErrorString(newObject->GetTypeName() + " is not a Node subclass, could not add as a child");
        return nullptr;
    }

    AddChild(child);
	child->ParentCallBack();

    return child;
}

ANode* ANode::CreateChild(FStringHash childType, const FString& childName)
{
    return CreateChild(childType, childName.CString());
}

ANode* ANode::CreateChild(FStringHash childType, const char* childName)
{
    ANode* child = CreateChild(childType);
    if (child)
        child->SetName(childName);
    return child;
}

void ANode::AddChild(ANode* child)
{
    // Check for illegal or redundant parent assignment
    if (!child || child->_parent == this)
        return;
    
    if (child == this)
    {
        ErrorString("Attempted parenting node to self");
        return;
    }

    // Check for possible cyclic parent assignment
    ANode* current = _parent;
    while (current)
    {
        if (current == child)
        {
            ErrorString("Attempted cyclic node parenting");
            return;
        }
        current = current->_parent;
    }

    ANode* oldParent = child->_parent;
    if (oldParent)
        oldParent->_children.Remove(child);
    _children.Push(child);
    child->_parent = this;
    child->OnParentSet(this, oldParent);
	if (_scenes)
	{
		_scenes->AddNode(child);
		if (child->GetType() == ACamera::GetTypeStatic())
		{
			_scenes->AddCamera(dynamic_cast<ACamera*>(child));
		}
	}
        
}

void ANode::RemoveChild(ANode* child)
{
    if (!child || child->_parent != this)
        return;

    for (size_t i = 0; i < _children.Size(); ++i)
    {
        if (_children[i] == child)
        {
            RemoveChild(i);
            break;
        }
    }
}

void ANode::RemoveChild(size_t index)
{
    if (index >= _children.Size())
        return;

    ANode* child = _children[index];
    // Detach from both the parent and the scene (removes _id assignment)
    child->_parent = nullptr;
    child->OnParentSet(this, nullptr);
    if (_scenes)
        _scenes->RemoveNode(child);
    _children.Erase(index);
}

void ANode::RemoveAllChildren()
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        child->_parent = nullptr;
        child->OnParentSet(this, nullptr);
        if (_scenes)
            _scenes->RemoveNode(child);
        it->Reset();
    }

    _children.Clear();
}

void ANode::RemoveSelf()
{
    if (_parent)
        _parent->RemoveChild(this);
    else
        delete this;
}

const FString& ANode::GetLayerName() const
{
    if (!_scenes)
        return FString::EMPTY;

    const TVector<FString>& layerNames = _scenes->LayerNames();
    return _layer < layerNames.Size() ? layerNames[_layer] : FString::EMPTY;
}

const FString& ANode::GetTagName() const
{
    if (!_scenes)
        return FString::EMPTY;

    const TVector<FString>& tagNames = _scenes->TagNames();
    return _tag < tagNames.Size() ? tagNames[_layer] : FString::EMPTY;
}

size_t ANode::NumPersistentChildren() const
{
    size_t ret = 0;

    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (!child->IsTemporary())
            ++ret;
    }

    return ret;
}

void ANode::AllChildren(TVector<ANode*>& result) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        result.Push(child);
        child->AllChildren(result);
    }
}

ANode* ANode::FindChild(const FString& childName, bool recursive) const
{
    return FindChild(childName.CString(), recursive);
}

ANode* ANode::FindChild(const char* childName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->_name == childName)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChild(childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChild(FStringHash childType, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetType() == childType)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChild(childType, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChild(FStringHash childType, const FString& childName, bool recursive) const
{
    return FindChild(childType, childName.CString(), recursive);
}

ANode* ANode::FindChild(FStringHash childType, const char* childName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetType() == childType && child->_name == childName)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChild(childType, childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildByLayer(unsigned layerMask, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetLayerMask() && layerMask)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChildByLayer(layerMask, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildByTag(unsigned char tag, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->_tag == tag)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChildByTag(tag, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildByTag(const FString& tagName, bool recursive) const
{
    return FindChildByTag(tagName.CString(), recursive);
}

ANode* ANode::FindChildByTag(const char* tagName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChildByTag(tagName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

void ANode::FindChildren(TVector<ANode*>& result, FStringHash childType, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetType() == childType)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildren(result, childType, recursive);
    }
}

void ANode::FindChildrenByLayer(TVector<ANode*>& result, unsigned layerMask, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetLayerMask() & layerMask)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByLayer(result, layerMask, recursive);
    }
}

void ANode::FindChildrenByTag(TVector<ANode*>& result, unsigned char tag, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->_tag == tag)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByTag(result, tag, recursive);
    }
}

void ANode::FindChildrenByTag(TVector<ANode*>& result, const FString& tagName, bool recursive) const
{
    FindChildrenByTag(result, tagName.CString(), recursive);
}

void ANode::FindChildrenByTag(TVector<ANode*>& result, const char* tagName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByTag(result, tagName, recursive);
    }
}

void ANode::SetScene(AScene* newScene)
{
    AScene* oldScene = _scenes;
    _scenes = newScene;
    OnSceneSet(_scenes, oldScene);
}

void ANode::SetId(unsigned newId)
{
    _id = newId;
}

void ANode::SkipHierarchy(FStream& source)
{
    ASerializable::Skip(source);

    size_t numChildren = source.ReadVLE();
    for (size_t i = 0; i < numChildren; ++i)
    {
        source.Read<FStringHash>(); // FStringHash childType
        source.Read<unsigned>(); // unsigned childId
        SkipHierarchy(source);
    }
}

void ANode::OnParentSet(ANode*, ANode*)
{
}

void ANode::OnSceneSet(AScene*, AScene*)
{
}

void ANode::OnSetEnabled(bool)
{
}

}
