#pragma once
class Mesh
{
private:

	ComPtr<ID3D12Resource> _vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView{ };
	uint32 _vertexCount{ 0 };

	Transform _transform{ };

public:

	void Init(vector<Vertex>& vec);

	void Render();

	void setTransform(const Transform& t) { _transform = t; }
};

