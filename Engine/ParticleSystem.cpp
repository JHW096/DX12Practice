#include "pch.h"
#include "ParticleSystem.h"
#include "StructuredBuffer.h"
#include "Mesh.h"
#include "Resources.h"
#include "Transform.h"
#include "Timer.h"

ParticleSystem::ParticleSystem() : Component(COMPONENT_TYPE::PARTICLE_SYSTEM)
{
	/*
	* 정보를 cpu에 넣는게 아니라는 점이 핵심이당.
	*/
	_particleBuffer = make_shared<StructuredBuffer>();
	//maxParticle갯수만큼 ParticleInfo size의 크기로 만들어달라.
	_particleBuffer->Init(sizeof(ParticleInfo), _maxParticle);

	_computeSharedBuffer = make_shared<StructuredBuffer>();
	_computeSharedBuffer->Init(sizeof(ComputeSharedInfo), 1);

	
	_mesh = GET_SINGLE(Resources)->LoadPointMesh();
	_material = GET_SINGLE(Resources)->Get<Material>(L"Particle");
	shared_ptr<Texture> tex = GET_SINGLE(Resources)->Load<Texture>(
		L"Bubbles", L"..\\Resource\\Texture\\Particle\\bubble.jpg"
		);

	_material->setTexture(0, tex);

	//TODO Material Class
	_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeParticle");
}

ParticleSystem::~ParticleSystem()
{
	
}

void ParticleSystem::FinalUpdate()
{
	_accTime += DELTA_TIME;

	int32 add = 0;
	if (_createInterval < _accTime)
	{
		_accTime = _accTime - _createInterval;
		add = 1;
	}

	_particleBuffer->PushComputeUAVData(UAV_REGISTER::u0);
	_computeSharedBuffer->PushComputeUAVData(UAV_REGISTER::u1);

	_computeMaterial->setInt(0, _maxParticle);
	_computeMaterial->setInt(1, add);


	_computeMaterial->setVec2(1, Vec2(DELTA_TIME, _accTime));
	_computeMaterial->setVec4(0, Vec4(_minLifeTime, _maxLifeTime, _minSpeed, _maxSpeed));

	//particle.fx의 ComputeShader부분으로 빵!
	_computeMaterial->Dispatch(1, 1, 1);
}

void ParticleSystem::Render()
{
	GetTransform()->PushData(); 

	_particleBuffer->PushGraphicsData(SRV_REGISTER::t9);
	_material->setFloat(0, _startScale);
	_material->setFloat(1, _endScale);
	_material->PushGraphicsData();

	_mesh->Render(_maxParticle);
}
