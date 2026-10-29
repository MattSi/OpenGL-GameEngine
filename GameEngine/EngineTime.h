#pragma once

class Engine;

class Time : public Singleton<Time>
{
public:
	
	void CleanUp() override;
	
	float DeltaTime() const { return deltaTime.count(); }	
	float TotalTime() const { return totalTime.count(); }
	int FrameCount() const { return frameCount; }

private:
	int frameCount = 0;
	std::chrono::duration<float> deltaTime = std::chrono::duration<float>(0);
	std::chrono::time_point<std::chrono::system_clock> beginTime = std::chrono::time_point<std::chrono::system_clock>{};
	std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::time_point<std::chrono::system_clock>{};
	std::chrono::duration<float> totalTime = std::chrono::duration<float>(0);

	void Initialize() ;
	void Update();

	inline explicit Time() = default;
	inline ~Time() = default;
	inline explicit Time(const Time&) = delete;
	Time& operator=(const Time&) = delete;
	
	friend class Singleton<Time>;
	friend class Engine;
};

