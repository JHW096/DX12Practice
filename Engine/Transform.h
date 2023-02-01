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

	const Vec3& GetLocalPosition() { return _localPosition; }
	const Vec3& GetLocalRotation() { return _localRotation; }
	const Vec3& GetLocalScale() { return _localScale; }

	//TEMP
	float GetBoundingSphereRadius() { return max(max(_localScale.x, _localScale.y), _localScale.z); }

	const Matrix& GetLocalToWorldMatrix() { return _matWorld; }
	Vec3 GetWorldPosition() { return _matWorld.Translation(); }

	
	//standard, world coordinate 
	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp() { return _matWorld.Up(); }
	//reason why Backward = LookVec : Right-coordinates
	Vec3 GetLook() { return _matWorld.Backward(); }

	void SetLocalPosition(const Vec3& position) { _localPosition = position; }
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; }
	void SetLocalScale(const Vec3& scale) { _localScale = scale; }

	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform> GetParent() { return _parent; }

public:

	virtual void FinalUpdate() override;
	void PushData();

};

