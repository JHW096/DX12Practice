#pragma once
#include "Component.h"

class Mesh;
class Material;

class MeshRenderer : public Component
{
private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;

public:

	MeshRenderer();
	virtual ~MeshRenderer();

	shared_ptr<Material> GetMaterial() { return _material; }

	void setMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void setMaterial(shared_ptr<Material> material) { _material = material; }

	

	void Render();

};

