#include "EngineCore.h"
#include "AssetManager.h"
#include "Asset.h"	

template<> AssetManager* Singleton<AssetManager>::instance = nullptr;

void AssetManager::Initialize()
{
}

void AssetManager::CleanUp()
{
	for (auto asset : assets) {
		asset->Destroy();
		delete asset;
	}
	assets.clear();
}

