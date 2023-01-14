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

	void setMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void setMaterial(shared_ptr<Material> material) { _material = material; }

	virtual void Update() override { Render(); }

	void Render();

};

