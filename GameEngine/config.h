#pragma once


struct RenderConfig
{
	std::string name = "Main Window";
	int width = 1280;
	int height = 1024;
	bool fullscreen = false;
};

struct SceneConfig
{
	std::string defaultScene = "Scene01.json";
};

struct AppConfig
{
	RenderConfig renderConfig;
	SceneConfig sceneConfig;
};
