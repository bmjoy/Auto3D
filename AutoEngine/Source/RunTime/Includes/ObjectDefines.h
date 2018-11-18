#pragma once
// Every non-abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_CLASS(_This,_Base) \
public: \
	using This = _This;\
	using Super = _Base;\
	~_This (); \
	virtual int GetClassID() { return ClassID (_This); }\
	virtual bool IsAbstract() { return GetRTTIStatic()->GetIsAbstract();}\
	virtual char* GetClassCstrName() { return #_This; }\
	virtual STRING GetClassString() { return #_This; } \
	virtual Auto3D::RTTI* GetRTTI() { return GetRTTIStatic(); }\
	static int GetClassIDStatic() { return ClassID(_This); }\
	static bool IsAbstractStatic () { return GetRTTIStatic()->GetIsAbstract();}\
	static char* GetClassCstrNameStatic () { return #_This; }\
	static STRING GetClassStringStatic() { return #_This; } \
	static Auto3D::RTTI* GetRTTIStatic() { static Auto3D::RTTI RTTIStatic(#_This, _Base::GetRTTIStatic(), ClassID(_This), false);return &RTTIStatic;}\

// Every abstract class that is derived from object has to place this inside the class Declaration
#define	REGISTER_DERIVED_ABSTRACT_CLASS(_This, _Base) \
public: \
	using This = _This; \
	using Super = _Base; \
	virtual ~_This (); \
	virtual int GetClassID() { return ClassID (_This); }\
	virtual bool IsAbstract() { return GetRTTIStatic()->GetIsAbstract();}\
	virtual char* GetClassCstrName() { return #_This; }\
	virtual STRING GetClassString() { return #_This; } \
	virtual Auto3D::RTTI* GetRTTI() { return GetRTTIStatic(); }\
	static int GetClassIDStatic() { return ClassID(_This); }\
	static bool IsAbstractStatic () { return GetRTTIStatic()->GetIsAbstract();}\
	static char* GetClassCstrNameStatic () { return #_This; }\
	static STRING GetClassStringStatic() { return #_This; } \
	static Auto3D::RTTI* GetRTTIStatic() { static Auto3D::RTTI RTTIStatic(#_This, _Base::GetRTTIStatic(), ClassID(_This), true);return &RTTIStatic;}\

// Should be placed in every serializable object derived class
#define DECLARE_OBJECT_SERIALIZE(_This)\
public: \
