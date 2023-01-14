#include "pch.h"
#include "MonoBehaviour.h"

//Component의 기본생성자가 없기 때문에
MonoBehaviour::MonoBehaviour() : Component(COMPONENT_TYPE::MONO_BEHAVIOUR)
{
}

MonoBehaviour::~MonoBehaviour()
{
}
