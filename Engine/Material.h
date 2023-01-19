#pragma once

class Shader;
class Texture;

enum
{
	MATERIAL_INT_COUNT = 5,
	MATERIAL_FLOAT_COUNT = 5,
	MATERIAL_TEXTURE_COUNT = 5,
};

struct MaterialParams
{
	array<int32, MATERIAL_INT_COUNT> intParams;
	array<float, MATERIAL_FLOAT_COUNT> floatParams;

	void setInt(uint8 index, int32 value) { intParams[index] = value; }
	void setFloat(uint8 index, float value) { floatParams[index] = value; }

};

class Material
{
private:

	shared_ptr<Shader> _shader;
	MaterialParams _params;
	array<shared_ptr<Texture>, MATERIAL_TEXTURE_COUNT> _textures;


public:

	shared_ptr<Shader> getShader() { return _shader; }

	void setShader(shared_ptr<Shader> shader) { _shader = shader; }
	void setInt(uint8 index, int32 value) { _params.setInt(index, value); }
	void setFloat(uint8 index, float value) { _params.setFloat(index, value); }
	void setTexture(uint8 index, shared_ptr<Texture> texture) { _textures[index] = texture; }
	
	void PushData();
};

