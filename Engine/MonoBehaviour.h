#pragma once
#include "Component.h"

class MonoBehaviour : public Component
{
private:

	virtual void FinalUpdate() sealed { }

public:

	MonoBehaviour();
	virtual ~MonoBehaviour();

};

