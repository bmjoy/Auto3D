#pragma once
#include "Texture.h"

namespace Auto3D {
class Texture2D : public Texture
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Texture2D, Texture)
public:
	typedef struct _TexParams {
		GLuint    minFilter;
		GLuint    magFilter;
		GLuint    wrapS;
		GLuint    wrapT;
	}TexParams;

public:
	explicit Texture2D(SharedPtr<Ambient> ambient);
	
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw() {}

};

}
