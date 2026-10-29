#pragma once
#include "Component.h"

class SkyboxComponent : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(SkyboxComponent, Component);

public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Load(json::JSON& data) override;

private:	
	std::array<std::string, 6> faces;
};

