#pragma once

struct ToolWindowData
{
	float ShipRotationRate = 0.0f;
	float SpecularStrength = 1.0f;
	glm::vec3 SpecularColor = glm::vec3(1.0f);
	bool IsMoveLight = false;


	bool Translate = false;
	bool Rotate = false;
	bool Scale = false;
	bool IsTransform = false;

	float Frequency = 0.0f;
	float Amplitude = 0.0f;
	bool WireframeRendering = false;
	bool TintBlue = false;
	bool isWaterScene = false;

	bool isSpaceScene = false;

	float rotationOffsetY = 0.0f;
	float rotationOffsetX = 0.0f;
	float rotationOffsetZ = 0.0f;

};


class Engine : public Singleton<Engine>
{
public:
	void Initialize();
	void CleanUp() override;
	void GameLoop();

	ToolWindowData toolWindowData;
private:
	Engine()=default;
	virtual ~Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	bool ShouldQuit() const;

	friend class Singleton<Engine>;
	int indent = 0;
	bool quit = false;

	
};

