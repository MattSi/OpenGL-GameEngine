#pragma once

class Asset;
class Engine;

class AssetManager : public Singleton<AssetManager>
{
public:

private:
	std::vector<Asset*> assets;
	friend class Singleton<AssetManager>;
	friend class Engine;

	void Initialize() ;
	void CleanUp() override;
	void Load(const json::JSON& data);
	AssetManager() = default;
	~AssetManager() = default;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

};

