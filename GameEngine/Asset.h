#pragma once
#include "Object.h"

class AssetManager;
class Asset : public Object
{
private:
	friend class AssetManager;

protected:
	Asset() = default;
	virtual ~Asset() = default;
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Load(json::JSON& data) override;
};

