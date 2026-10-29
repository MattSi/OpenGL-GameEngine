#pragma once
#include "Component.h"
#include "IRenderable.h"


class Model;
class ModelRenderer : public Component, public IRenderable
{
    DECLARE_DYNAMIC_DERIVED_CLASS(ModelRenderer, Component);
public:
        virtual void Load(json::JSON& data) override;
        virtual void Destroy() override;
        virtual void Initialize() override;

        Model* GetModel() const { return model; }
        void SetModel(const std::string& path) {
			modelPath = path;
        }

        void SetModel(Model* mdl);
        const std::string& GetModelPath() const { return modelPath; }
        bool visible = true;


        // view and projection matrices are passed from the Render System
        void Render(const glm::mat4& view, const glm::mat4& proj, float delta) override;
        void Render(float delta) override
        {
            // Not used in this implementation
        }
        const char* GetTypeName() const override { return "ModelRenderer"; }

        float overrideSpecularStrength = 0.0f;
        bool hasSpecularStrengthOverride = false;

        glm::vec3 overrideColor = glm::vec3(1.0f, 1.0f, 1.0f);
        bool hasColorOverride = false;

		bool hasSpecularColorOverride = false;
		glm::vec3 overrideSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    private:
        void ApplyOverridesToModel();
        bool TryLoadModel(const std::string& path);
        class Model* model = nullptr;
        std::string modelPath;

        // Used when modelPath cannot be loaded (e.g. licensed assets not shipped in the repo).
        std::string fallbackModelPath;
        bool hasFallbackScale = false;
        glm::vec3 fallbackScale = glm::vec3(1.0f);
        bool ownModel = false;
		
        float overrideEmission = 0.0f;
		bool hasEmissionOverride = false;
		
};

