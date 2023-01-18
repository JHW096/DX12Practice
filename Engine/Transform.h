#pragma once
#include "Component.h"

class Transform : public Component
{
private:

	Vec3 _localPosition{ };
	Vec3 _localRotation{ };
	Vec3 _localScale{ 1.0f, 1.0f, 1.0f };

	Matrix _matLocal{ };
	Matrix _matWorld{ };

	weak_ptr<Transform> _parent;

public:

	Transform();
	virtual ~Transform();

public:

	const Vec3& GetLocalPosition() const { return _localPosition; }
	const Vec3& GetLocalRotation() const { return _localRotation; }
	const Vec3& GetLocalScale() const { return _localScale; }

	const Matrix& GetLocalToWorldMatrix() const { return _matWorld; }
	const Vec3& GetWolrdPosition() const { return _matWorld.Translation(); }

	
	//standard, world coordinate 
	Vec3 GetRight() const { return _matWorld.Right(); }
	Vec3 GetUp() const { return _matWorld.Up(); }
	//reason why Backward = LookVec : Right-coordinates
	Vec3 GetLook() const { return _matWorld.Backward(); }

	void SetLocalPosition(const Vec3& position) { _localPosition = position; }
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; }
	void SetLocalScale(const Vec3& scale) { _localScale = scale; }

	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform> GetParent() { return _parent; }

public:

	virtual void FinalUpdate() override;
	void PushData();

};

