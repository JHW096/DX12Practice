#pragma once
#include "Object.h"

class Shader;
class Texture;

enum
{
	MATERIAL_INT_COUNT = 4,
	MATERIAL_FLOAT_COUNT = 4,
	MATERIAL_TEXTURE_COUNT = 4,
	MATERIAL_VECTOR2_COUNT = 4,
	MATERIAL_VECTOR4_COUNT = 4,
};

struct MaterialParams
{
	array<int32, MATERIAL_INT_COUNT> intParams;
	array<float, MATERIAL_FLOAT_COUNT> floatParams;
	array<int32, MATERIAL_TEXTURE_COUNT> texOnParams;
	array<Vec2, MATERIAL_VECTOR2_COUNT> vec2Params;
	array<Vec4, MATERIAL_VECTOR4_COUNT> vec4Params;

	void setInt(uint8 index, int32 value) { intParams[index] = value; }
	void setFloat(uint8 index, float value) { floatParams[index] = value; }
	void setTexOn(uint8 index, int32 value) { texOnParams[index] = value; }
	void setVec2(uint8 index, Vec2 value) { vec2Params[index] = value; }
	void setVec4(uint8 index, Vec4 value) { vec4Params[index] = value; }
};

class Material : public Object
{
private:

	shared_ptr<Shader> _shader;
	MaterialParams _params;
	array<shared_ptr<Texture>, MATERIAL_TEXTURE_COUNT> _textures;

public:

	Material();
	virtual ~Material();

public:

	shared_ptr<Shader> GetShader() { return _shader; }

	void setShader(shared_ptr<Shader> shader) { _shader = shader; }
	void setInt(uint8 index, int32 value) { _params.setInt(index, value); }
	void setFloat(uint8 index, float value) { _params.setFloat(index, value); }
	void setTexture(uint8 index, shared_ptr<Texture> texture) 
	{
		_textures[index] = texture;
		_params.setTexOn(index, (texture == nullptr ? 0 : 1));
	}
	
	void setVec2(uint8 index, Vec2 value) { _params.setVec2(index, value); }
	void setVec4(uint8 index, Vec4 value) { _params.setVec4(index, value); }

	void PushGraphicsData();
	void PushComputeData();
	void Dispatch(uint32 x, uint32 y, uint32 z);

	shared_ptr<Material> Clone();

};

