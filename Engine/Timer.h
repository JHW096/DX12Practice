#pragma once

class Timer
{
	DECLARE_SINGLE(Timer);
private:

	uint64 _frequency{ 0 };
	uint64 _prevCount{ 0 };
	float _deltaTime{ 0.0f }; 

private: // frame

	uint32 _frameCount{ 0 };
	float _frameTime{ 0.0f };
	uint32 _fps{ 0 };

public:

	void Init();
	void Update();


	//frame per second
	uint32 getFps() { return _fps; }

	//prevFrame ~ prsnFrame time
	float GetDeltaTime() { return _deltaTime; }

};

