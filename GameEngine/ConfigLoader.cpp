#include "EngineCore.h"
#include "ConfigLoader.h"


AppConfig ConfigLoader::LoadAppConfig(const std::string& filename)
{
	
	std::ifstream file(filename);
	M_ASSERT(file.is_open(), ("Failed to open settings file: " + filename).c_str());

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();
	json::JSON root = json::JSON::Load(content);

	AppConfig appConfig;

	if (root.hasKey("RenderSystem"))
	{
		auto renderCfg = root.at("RenderSystem");
		if (renderCfg.hasKey("name"))
			appConfig.renderConfig.name = renderCfg.at("name").ToString();
		if (renderCfg.hasKey("width"))
			appConfig.renderConfig.width = static_cast<int>(renderCfg.at("width").ToInt());
		if (renderCfg.hasKey("height"))
			appConfig.renderConfig.height = static_cast<int>(renderCfg.at("height").ToInt());
		if (renderCfg.hasKey("fullScreen"))
			appConfig.renderConfig.fullscreen = renderCfg.at("fullScreen").ToInt() != 0;
	}
	if (root.hasKey("DefaultScene"))
	{
		appConfig.sceneConfig.defaultScene = root.at("DefaultScene").ToString();
	}
	
	return appConfig;
}
