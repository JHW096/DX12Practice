#pragma once
#include "Object.h"
//class Texture;
class Material;

class Mesh : public Object
{
private:

	ComPtr<ID3D12Resource> _vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView{ };
	uint32 _vertexCount{ 0 };

	ComPtr<ID3D12Resource> _indexBuffer;
	D3D12_INDEX_BUFFER_VIEW _indexBufferView;
	uint32 _indexCount{ };
	
	//test
	//shared_ptr<Texture> _tex{ };


private:

	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);


public:

	Mesh();
	virtual ~Mesh();

public:

	void Init(vector<Vertex>& vertexBuffer, vector<uint32>& indexBuffer);

	void Render(uint32 instanceCount = 1);


	//Test 
	//void setTexture(shared_ptr<Texture> tex) { _tex = tex;  }
};

