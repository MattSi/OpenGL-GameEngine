#include "EngineCore.h"
#include "EngineTime.h"


template<> Time* Singleton<Time>::instance = nullptr;

void Time::Initialize()
{
	DEBUG_LOG(0, "EngineTime::Initialize");
	
	beginTime = std::chrono::system_clock::now();
	endTime = std::chrono::system_clock::now();

	deltaTime = std::chrono::duration<float>(0);
	totalTime = std::chrono::duration<float>(0);
	frameCount = 0;
	
}

void Time::CleanUp()
{
	DEBUG_LOG(0, "EngineTime::CleanUp");
	frameCount = 0;
	deltaTime = std::chrono::duration<float>(0);
	totalTime = std::chrono::duration<float>(0);

	beginTime = std::chrono::time_point<std::chrono::system_clock>{};
	endTime = std::chrono::time_point<std::chrono::system_clock>{};
}


void Time::Update()
{
	endTime = std::chrono::system_clock::now();
	deltaTime = endTime - beginTime;
	beginTime = endTime;
	totalTime += deltaTime;
	frameCount++;
}

