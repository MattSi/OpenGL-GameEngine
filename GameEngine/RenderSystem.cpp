#include "EngineCore.h"
#include "RenderSystem.h"
#include "IRenderable.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include "LightComponent.h"
#include "Shader.h"
#include "Camera.h"
#include "CameraComponent.h"
#include "CameraTweenScript.h"
#include "PrimitiveMeshes.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "FlyingComponent.h"
#include "PostProcessManager.h"
#include "Framebuffer.h"
#include "GrayscalePass.h"
#include "InversionPass.h"
#include "KernelBlurPass.h"
#include "KernelSharpenPass.h"
#include "KernelEdgepass.h"
#include "PostFogPass.h"
#include "DepthOfFieldPass.h"
#include "PhysicalDoFPass.h"
#include "DepthBasedColorGradingPass.h"
#include "InputManager.h"
#include "EngineTime.h"
#include "WaterScenePass.h"
#include "Engine.h"
#include "ModelRenderer.h"


template<> RenderSystem* Singleton<RenderSystem>::instance = nullptr;
 
void RenderSystem::Initialize(const RenderConfig& cfg)
{
	name = cfg.name;
	width = cfg.width;
	height = cfg.height;
	fullScreen = cfg.fullscreen;

	auto setAttribute = [](SDL_GLattr attr, int value) {
			if (SDL_GL_SetAttribute(attr, value) != 0) {
				throw std::runtime_error(std::string("Failed to set SDL GL attribute: ") + SDL_GetError());
			}
		};

	setAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	setAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	setAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	setAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef DEBUG
	setAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if (!cfg.fullscreen) {
		windowFlags |= SDL_WINDOW_RESIZABLE;
	}
	if (fullScreen) {
		SDL_DisplayMode displayMode;
		if(SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
			width = displayMode.w;
			height = displayMode.h;
		}
		windowFlags |= SDL_WINDOW_FULLSCREEN;
	}

	window = SDL_CreateWindow(
		name.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		windowFlags
	);

	if (!window) {
		throw std::runtime_error(std::string("Failed to create SDL window: ") + SDL_GetError());
	}

	glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		SDL_DestroyWindow(window);
		window = nullptr;
		throw std::runtime_error(std::string("Failed to create SDL GL context: ") + SDL_GetError());
	}

	if(SDL_GL_MakeCurrent(window, glContext)!=0) {
		SDL_GL_DeleteContext(glContext);
		glContext = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		throw std::runtime_error(std::string("Failed to make GL context current: ") + SDL_GetError());
	}

	SDL_GL_SetSwapInterval(1); // Enable VSync

	glewExperimental = GL_TRUE;
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		std::string err = reinterpret_cast<const char*>(glewGetErrorString(glewStatus));
		SDL_GL_DeleteContext(glContext);
		glContext = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		throw std::runtime_error("Failed to initialize GLEW" + err);
	}

	glGetError(); // Clear GLEW's error
	glViewport(0, 0, width, height);
	InitOpenGLState();
	std::cout << "RenderSystem initialized with SDL window + GLEW\n";

	sceneFramebuffer = new Framebuffer();
	sceneFramebuffer->create(width, height, true, GL_RGBA16F);

	PostProcessManager::GetInstance().Initialize(width, height);

	// Create ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf", 16.0f);
	//io.Fonts->Build();

	ImGui::StyleColorsDark();

	const char* glsl_version = "#version 330";
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init(glsl_version);

	lightingShader = new Shader();
	lightingShader->name = "LightingShader";
	if (!lightingShader->LoadFromFiles("Assets/Shaders/model.vert", "Assets/Shaders/model.frag"))
	{
		throw std::runtime_error("Unable to load lighting Shader");

	}

	colorByPositionShader = new Shader();
	colorByPositionShader->name = "ColorByPositionShader";
	if (!colorByPositionShader->LoadFromFiles("Assets/Shaders/colorbypos.vert", "Assets/Shaders/colorbypos.frag"))
	{
		throw std::runtime_error("Unable to load Color by Position Shader");
	}

	defaultShader = lightingShader;

	cubeShader = new Shader();
	cubeShader->name = "CubeShader";
	if (!cubeShader->LoadFromFiles("Assets/Shaders/cube.vert", "Assets/Shaders/cube.frag"))
	{
		throw std::runtime_error("Unable to load Cube Shader");
	}

	skyboxShader = new Shader();
	skyboxShader->name = "SkyboxShader";
	if (!skyboxShader->LoadFromFiles("Assets/Shaders/skybox.vert", "Assets/Shaders/skybox.frag"))
	{
		throw std::runtime_error("Unable to load Skybox Shader");
	}
	skyboxMesh = PrimitiveMeshes::GetCubeMesh();

	Mesh* fullscreenQuad = PrimitiveMeshes::GetFullscreenQuad();
	grayscaleShader = new Shader();
	grayscaleShader->name = "GrayscalePost";
	if (!grayscaleShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/post_grayscale.frag"))
	{
		throw std::runtime_error("Unable to load Grayscale Post-Process Shader");
	}

	

	invertShader = new Shader();
	invertShader->name = "InversionPost";
	if (!invertShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/post_inversion.frag"))
	{
		throw std::runtime_error("Unable to load Inversion Post-Process Shader");
	}

	kernelBlurShader = new Shader();
	kernelBlurShader->name = "KernelBlurPost";
	if (!kernelBlurShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/post_kernel_blur.frag"))
	{
		throw std::runtime_error("Unable to load Kernel Blur Post-Process Shader");
	}

	kernelSharpenShader = new Shader();
	kernelSharpenShader->name = "KernelSharpenPost";
	if (!kernelSharpenShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/post_kernel_sharpen.frag"))
	{
		throw std::runtime_error("Unable to load Kernel Sharpen Post-Process Shader");
	}

	edgeDetectShader = new Shader();
	edgeDetectShader->name = "EdgeDetectPost";
	if (!edgeDetectShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/post_kernel_edge.frag"))
	{
		throw std::runtime_error("Unable to load Edge Detect Post-Process Shader");
	}

	fogShader = new Shader();
	fogShader->name = "FogPost";
	if (!fogShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/post_fog.frag"))
	{
		throw std::runtime_error("Unable to load Fog Post-Process Shader");
	}

	dofShader = new Shader();
	dofShader->name = "DepthOfFieldPost";
	if (!dofShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/post_dof.frag"))
	{
		throw std::runtime_error("Unable to load Depth of Field Post-Process Shader");
	}

	physicalDoFShader = new Shader();
	physicalDoFShader->name = "PhysicalDepthOfFieldPost";
	if (!physicalDoFShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/post_dof_physical.frag"))
	{
		throw std::runtime_error("Unable to load Physical Depth of Field Post-Process Shader");
	}


	depthBasedColorGradingShader = new Shader();
	depthBasedColorGradingShader->name = "DepthBasedColorGradingPost";
	if (!depthBasedColorGradingShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/post_depthcolor.frag"))
	{
		throw std::runtime_error("Unable to load Depth Based Color Grading Post-Process Shader");
	}

	waterSceneShader = new Shader();
	waterSceneShader->name = "WaterSceneShader";
	if (!waterSceneShader->LoadFromFiles("Assets/Shaders/post_grayscale.vert", "Assets/Shaders/water_scene.frag"))
	{
		throw std::runtime_error("Unable to load Water Scene Shader");
	}
	  
	auto& ppm = PostProcessManager::GetInstance();
	ppm.AddPass(std::make_shared<GrayscalePass>(grayscaleShader, fullscreenQuad));
	ppm.AddPass(std::make_shared<InversionPass>(invertShader, fullscreenQuad));
	ppm.AddPass(std::make_shared<KernelBlurPass>(kernelBlurShader, fullscreenQuad));
	ppm.AddPass(std::make_shared<KernelSharpenPass>(kernelSharpenShader, fullscreenQuad));
	ppm.AddPass(std::make_shared<KernelEdgepass>(edgeDetectShader, fullscreenQuad));
	ppm.AddPass(std::make_shared<PostFogPass>(fogShader, fullscreenQuad));
	ppm.AddPass(std::make_shared<DepthOfFieldPass>(dofShader, fullscreenQuad));
	ppm.AddPass(std::make_shared<PhysicalDoFPass>(physicalDoFShader, fullscreenQuad));
	ppm.AddPass(std::make_shared<DepthBasedColorGradingPass>(depthBasedColorGradingShader, fullscreenQuad));
	ppm.AddPass(std::make_shared<WaterScenePass>(waterSceneShader, fullscreenQuad));
}

 
void RenderSystem::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	renderables.clear();
	lightComponents.clear();
	mainCameraComponent = nullptr;

	// Free post-process FBOs and shaders before the GL context goes away.
	PostProcessManager::GetInstance().Destroy();

	Shader** ownedShaders[] = {
		&lightingShader, &colorByPositionShader, &cubeShader, &skyboxShader,
		&grayscaleShader, &invertShader, &kernelBlurShader, &kernelSharpenShader,
		&edgeDetectShader, &fogShader, &dofShader, &physicalDoFShader,
		&depthBasedColorGradingShader, &waterSceneShader };
	for (Shader** s : ownedShaders) {
		delete *s;
		*s = nullptr;
	}
	defaultShader = nullptr;

	if (sceneFramebuffer)
	{
		delete sceneFramebuffer;
		sceneFramebuffer = nullptr;
	}

	if (glContext) {
		SDL_GL_DeleteContext(glContext);
		glContext = nullptr;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
}

void RenderSystem::LoadScene()
{
	DEBUG_LOG(indent, "RenderSystem::LoadScene");
	lightComponents.clear();

	const Scene* activeScene = SceneManager::GetInstance().GetActiveScene();
	if (!activeScene) {
		return;
	}
	for (auto* entity : activeScene->GetEntities()) {
		auto lights = entity->GetComponents<LightComponent>();
		for (LightComponent* light : lights) {
			this->lightComponents.push_back(light);
		}
	}
}


void RenderSystem::Update(float delta)
{
	if (!window || !glContext) {
		return;
	}

	// If no main camera, just clear the screen
	if (!mainCameraComponent) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(window);
		return;
	}


	viewMatrix = mainCameraComponent->GetCamera().GetViewMatrix();

	float aspect = (height != 0) ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;

	mainCameraComponent->GetCamera().SetPerspective(
		mainCameraComponent->GetCamera().GetFovY(),
		aspect, 
		mainCameraComponent->GetCamera().GetNearPlane(),
		mainCameraComponent->GetCamera().GetFarPlane());

	projectionMatrix = mainCameraComponent->GetCamera().GetProjectionMatrix();
	RenderAll(delta);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	 
	DrawDebugHUD();
	DrawToolWindow();  

	DrawCrosshair();
	PostProcessManager::GetInstance().OnImGui();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(window);
}


Entity* RenderSystem::CloneEntityShallow(Entity* original, const std::string& newName)
{
	if (!original) return nullptr;

	Scene* scene = SceneManager::GetInstance().GetActiveScene();
	if (!scene) return nullptr;

	Entity* clone = static_cast<Entity*>(CreateObject("Entity"));
	clone->SetName(newName);


	// Transform
	if (auto* srcT = original->GetComponent<Transform>())
	{
		Transform* transform = static_cast<Transform*>(CreateObject("Transform"));		
		transform->SetPosition(srcT->GetPosition());
		transform->SetRotation(srcT->GetRotation());
		transform->SetScale(srcT->GetScale());

		clone->AddComponent(transform);
	}

	if (auto* srcR = original->GetComponent<MeshRenderer>())
	{
		MeshRenderer* meshRenderer = static_cast<MeshRenderer*>(CreateObject("MeshRenderer"));
		
		meshRenderer->mesh = srcR->mesh;
		meshRenderer->material = srcR->material;

		clone->AddComponent(meshRenderer);
	}

	if(auto*srcR = original->GetComponent<ModelRenderer>())
	{
		ModelRenderer* modelRenderer = static_cast<ModelRenderer*>(CreateObject("ModelRenderer"));
		modelRenderer->SetModel(srcR->GetModelPath());
		clone->AddComponent(modelRenderer);
	}

	return clone;
}


void RenderSystem::FocusCameraOnEntity(Entity* targetEntity)
{
	if(!targetEntity || !mainCameraComponent) {
		return;
	}

	CameraTweenScript* tween = targetEntity->GetComponent<CameraTweenScript>();
	Entity* cameraEntity = mainCameraComponent->GetOwner();

	if(!tween) {
		tween = static_cast<CameraTweenScript*>(CreateObject("CameraTweenScript"));
		cameraEntity->AddComponent(tween);
	}
	
	tween->startPosition = mainCameraComponent->GetCamera().GetPosition();
	tween->targetPosition = targetEntity->GetPosition() + glm::vec3(0.0f, 0.0f, 25.0f);

	tween->startYawPitch = { mainCameraComponent->GetCamera().GetYaw(), mainCameraComponent->GetCamera().GetPitch() };

	glm::vec3 direction = glm::normalize(targetEntity->GetPosition() - tween->targetPosition);
	tween->targetYawPitch = {
		glm::degrees(atan2(direction.z, direction.x)),
		glm::degrees(asin(direction.y))
	};

	tween->duration = 1.0f;
	tween->StartTween();
}

void RenderSystem::DrawCrosshair()
{
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	if (!drawList) return;

	float cx = width * 0.5f;
	float cy = height * 0.5f;

	float size = 10.0f;  // 准星的“臂长”
	float gap = 4.0f;   // 中间空白
	float thickness = 2.0f;

	ImU32 col = IM_COL32(255, 255, 255, 255); // 白色

	// 横线左
	drawList->AddLine(ImVec2(cx - size, cy),
		ImVec2(cx - gap, cy),
		col, thickness);
	// 横线右
	drawList->AddLine(ImVec2(cx + gap, cy),
		ImVec2(cx + size, cy),
		col, thickness);

	// 竖线上
	drawList->AddLine(ImVec2(cx, cy - size),
		ImVec2(cx, cy - gap),
		col, thickness);
	// 竖线下
	drawList->AddLine(ImVec2(cx, cy + gap),
		ImVec2(cx, cy + size),
		col, thickness);
}

void RenderSystem::DrawDebugHUD()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.35f);

	ImGui::SetNextWindowSize(ImVec2(260.0f, 0.0f), ImGuiCond_Always);
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoTitleBar |
		//ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings;

	ImGui::Begin("DebugHUD", nullptr, flags);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));

	ImGui::Text( "Final Project");
	ImGui::Text("FPS: %.1f", 1.0f / Time::GetInstance().DeltaTime());

	InputManager& inputManager = InputManager::GetInstance();
	int mx, my;
	inputManager.GetMousePosition(mx, my);
	ImGui::Text("Mouse Pos: %.2f %.2f", (float)mx, (float)my);

	ImGui::Separator();
	ImGui::Text("Left Button:   %s", inputManager.IsMouseButtonDown(SDL_BUTTON_LEFT) ? "Down" : "Up");
	ImGui::Text("Middle Button: %s", inputManager.IsMouseButtonDown(SDL_BUTTON_MIDDLE) ? "Down" : "Up");
	ImGui::Text("Right Button:  %s", inputManager.IsMouseButtonDown(SDL_BUTTON_RIGHT) ? "Down" : "Up");

	ImGui::Separator();
	SceneManager& sceneManager = SceneManager::GetInstance();
	Entity* playerEntity = sceneManager.GetActiveScene()->FindEntityByName("AreaCraft Model");
	if (playerEntity) {
		const Transform* t = playerEntity->GetTransform();
		ImGui::Text("Fighter Position: {%.3f %.3f %.3f}",
			t->GetPosition().x,
			t->GetPosition().y,
			t->GetPosition().z);

		ImGui::Text("Rotation: {%.3f %.3f %.3f}",
			t->GetRotation().x,
			t->GetRotation().y,
			t->GetRotation().z);

		ImGui::Text("Scale: {%.3f %.3f %.3f}",
			t->GetScale().x,
			t->GetScale().y,
			t->GetScale().z);
	}
	

	ImGui::PopStyleColor();
	ImGui::End();
}

void RenderSystem::DrawToolWindow()
{
	Entity* selected = nullptr;
	// Default layout (first run / no imgui.ini): one column on the left,
	// Debug HUD at the top, then Tool Window, then Post Process.
	ImGui::SetNextWindowSize(ImVec2(300, 320), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(10, 250), ImGuiCond_FirstUseEver);

	ImGui::Begin("Tool Window");
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("FPS: %.1f (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
	ImGui::Separator();
	static int mode = 0;
	ImGui::Text("Hold the right mouse button to look around, \nand use WASD to move freely through the scene,\nand press E to move up or Q to move down");
	ImGui::Text("Double click an entity to focus");
	ImGui::Separator();
	ImGui::RadioButton("Move Entity (Click an entity to move)", &mode, 0);
	ImGui::RadioButton("Color By Position", &mode, 1);
	ImGui::RadioButton("Move Cubes To Entity (Double click an entity)", &mode, 2);
	currentToolMode = mode;
	ImGui::Separator();

	

	Scene* scene = SceneManager::GetInstance().GetActiveScene();
	const std::vector<Entity*>&  entities = scene->GetEntities();
	static int selectedEntityIndex = -1;
	ImGui::Text("Entities (%d):", (int)entities.size());
	ImGui::BeginChild("EntityList", ImVec2(0, 100), true);
	for (int i = 0; i < (int)entities.size(); ++i) {
		Entity* entity = entities[i];
		std::string name = entity->GetName();
		if(name.empty()) {
			name = "Entity " + std::to_string(i);
		}

		bool IsSelected = (i == selectedEntityIndex);
		if (ImGui::Selectable(name.c_str(), IsSelected)) {
			selectedEntityIndex = i;
		}

		if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) ) {			
			FocusCameraOnEntity(entity);
		}
	}
	ImGui::EndChild();
	
	if (selectedEntityIndex >= 0 && selectedEntityIndex < (int)entities.size())
	{
		selected = entities[selectedEntityIndex];
		Transform* t = selected->GetComponent<Transform>(); 
		if (t)
		{
			glm::vec3& pos = t->position;
			float step = 0.1f;      

			ImGui::Separator();
			ImGui::Text("Position");
			if (ImGui::Button("Reset"))
			{
				pos = t->initialPosition;
				t->rotation = t->initialRotation;
				t->scale = t->initialScale;
			}
			// X
			ImGui::PushID("PosX");			
			ImGui::Text("X: %.2f", pos.x);
			ImGui::SameLine();
			if (ImGui::Button("-")) pos.x -= step;
			ImGui::SameLine();
			ImGui::DragFloat("", &pos.x, step, -100.0f, 100.0f);
			ImGui::SameLine();
			if (ImGui::Button("+")) pos.x += step;
			ImGui::PopID();

			// Y
			ImGui::PushID("PosY");
			
			ImGui::Text("Y: %.2f", pos.y);
			ImGui::SameLine();
			if (ImGui::Button("-")) pos.y -= step;
			ImGui::SameLine();
			ImGui::DragFloat("", &pos.y, step, -100.0f, 100.0f);
			ImGui::SameLine();
			if (ImGui::Button("+")) pos.y += step;
			ImGui::PopID();

			// Z
			ImGui::PushID("PosZ");
			
			ImGui::Text("Z: %.2f", pos.z);
			ImGui::SameLine();
			if (ImGui::Button("-")) pos.z -= step;
			ImGui::SameLine();
			ImGui::DragFloat("", &pos.z, step, -100.0f, 100.0f);
			ImGui::SameLine();
			if (ImGui::Button("+")) pos.z += step;
			ImGui::PopID();

		
		}
	}

	if( mode ==2)
	{
		if(ImGui::Button("Fire"))
		{
			
			Scene* scene = SceneManager::GetInstance().GetActiveScene();
			if (!scene) return;
			Entity* prototype = scene->FindEntityByName("Spinning Cube2");
			//FireFromPrototype(prototype, selected);
			FireFromPrototypeForward(prototype);
		}
	}
	ImGui::End();
}

void RenderSystem::FireFromPrototypeForward(Entity* prototype)
{
	if (!prototype) {
		std::cout << "[Fire Forward] prototype is null\n";
		return;
	}

	Scene* scene = SceneManager::GetInstance().GetActiveScene();
	if (!scene) return;

	CameraComponent* camComp = GetMainCamera();
	if (!camComp) return;


	Entity* cube = CloneEntityShallow(prototype, "Projectile");
	if (!cube) return;
	scene->AddEntity(cube);


	glm::vec3 camPos = camComp->GetCamera().GetPosition();
	glm::vec3 camForward = camComp->GetCamera().GetForward(); // 


	glm::vec3 spawnPos = camPos + camForward * 2.5f;

	
	spawnPos.x += (float(rand()) / RAND_MAX - 0.5f) * 0.3f;
	spawnPos.y += (float(rand()) / RAND_MAX - 0.5f) * 0.3f;

	if (auto* t = cube->GetComponent<Transform>())
	{
		t->SetPosition(spawnPos);
		t->SetScale(glm::vec3(0.5f));
		
	}

	
	glm::vec3 targetPos = camPos + camForward * 200.0f; 

	
	FlyingComponent* flyComp = static_cast<FlyingComponent*>(CreateObject("FlyingComponent"));
	flyComp->Start(targetPos, 50.0f);   
	cube->AddComponent(flyComp);

	std::cout << "[Fire Forward] Projectile spawned\n";
}

void RenderSystem::FireFromPrototype(Entity* prototype, Entity* target)
{
	if (!prototype) {
		std::cout << "[Fire] prototype is null\n";
		return;
	}

	Scene* scene = SceneManager::GetInstance().GetActiveScene();
	if (!scene) return;



	Entity* cube = CloneEntityShallow(prototype, "Projectile");
	if (!cube) return;
	scene->AddEntity(cube);


	//glm::vec3 spawnPos = glm::vec3(1.0f, 1.0f, -151.0f);
	

	//spawnPos.x += (float(rand()) / RAND_MAX - 0.5f) * 0.6f;
	//spawnPos.y += (float(rand()) / RAND_MAX - 0.5f) * 0.6f;
	//spawnPos.z += (float(rand()) / RAND_MAX - 0.5f) * 0.6f;
	//if (auto* t = cube->GetComponent<Transform>())
	//{
	//	t->SetPosition(spawnPos);
	//	t->SetScale(glm::vec3(1.0f)); //
	//}


	glm::vec3 targetPos = target->GetPosition();
	glm::vec3 spawnPos = targetPos + glm::vec3(0, 0, -150.0f);
	cube->SetPosition(spawnPos);
	cube->SetScale(glm::vec3(1.0f));

	FlyingComponent* flyComp = static_cast<FlyingComponent*>(CreateObject("FlyingComponent"));
	flyComp->Start(targetPos, 20.0f);
	cube->AddComponent(flyComp);

	std::cout << "[Fire] Projectile spawned\n";
}

void RenderSystem::FireFromPrototypeRandom(Entity* prototype)
{

	if (!prototype) {
		std::cout << "[Fire] prototype is null\n";
		return;
	}

	Scene* scene = SceneManager::GetInstance().GetActiveScene();
	if (!scene) return;

	Transform* protoTransform = prototype->GetComponent<Transform>();
	if(!protoTransform) {
		std::cout << "[Fire] prototype has no Transform\n";
		return;
	}

	glm::vec3 centerPos = protoTransform->GetPosition();

	Entity* cloneEntity = CloneEntityShallow(prototype, "Projectile");
	if (!cloneEntity) return;
	scene->AddEntity(cloneEntity);


	auto GenRandomPointInSphere = [&](float r)->glm::vec3 {
		glm::vec3 randomDir(
			float(rand()) / RAND_MAX  - 0.5f,
			float(rand()) / RAND_MAX  - 0.5f,
			float(rand()) / RAND_MAX  - 0.5f
		);

		if (glm::length(randomDir) > 0.0001f) {
			randomDir = glm::normalize(randomDir);
		}
		else {
			randomDir = glm::vec3(0.0f, 1.0f, 0.0f);
		}

		float randomDist = (float(rand()) / RAND_MAX) * r;
		return centerPos + randomDir * randomDist;
	};

	glm::vec3 startPos = GenRandomPointInSphere(50.0f);
	glm::vec3 targetPos = GenRandomPointInSphere(50.0f);
	DEBUG_LOG(2, "FireFromPrototypeRandom: StartPos {" +
		std::to_string(startPos.x) + ", " +
		std::to_string(startPos.y) + ", " +
		std::to_string(startPos.z) + "} " +
		" TargetPos {" +
		std::to_string(targetPos.x) + ", " +
		std::to_string(targetPos.y) + ", " +
		std::to_string(targetPos.z) + "} "
	);
	
	if (auto* t = cloneEntity->GetComponent<Transform>())
	{
		t->SetPosition(startPos);
		t->SetScale(glm::vec3(1.5f)); //
	}

	FlyingComponent* flyComp = static_cast<FlyingComponent*>(CreateObject("FlyingComponent"));
	flyComp->Start(targetPos, 4.0f);
	cloneEntity->AddComponent(flyComp);

	std::cout << "[Fire] Projectile spawned\n";
}

void RenderSystem::Load(const json::JSON& data)
{
	if(data.hasKey("name"))
		name = data.at("name").ToString();	
	if (data.hasKey("width"))
		width = data.at("width").ToInt();
	if (data.hasKey("height"))
		height = data.at("height").ToInt();
	if (data.hasKey("fullScreen"))
		fullScreen = data.at("fullScreen").ToBool();

	DEBUG_LOG(indent, "RenderSystem::Load: "  
		", Width: " + std::to_string(width) +
		", Height: " + std::to_string(height) +
		", FullScreen: " + (fullScreen ? "True" : "False"));
}

void RenderSystem::InitOpenGLState()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
}

void RenderSystem::OnResize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);
	mainCameraComponent->GetCamera().SetAspect(
		(height != 0) ? static_cast<float>(width) / static_cast<float>(height) : 1.0f);

	if (sceneFramebuffer)
	{
		sceneFramebuffer->resize(newWidth, newHeight, /*withDepth=*/true, GL_RGBA16F);
	}

	PostProcessManager::GetInstance().Resize(newWidth, newHeight);
}

void RenderSystem::SetSkyboxTextures(const std::array<std::string, 6>& faces)
{
	if (skyboxCubemap == 0)
	{
		glGenTextures(1, &skyboxCubemap);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap);

	stbi_set_flip_vertically_on_load(false);

	int w, h, channels;
	int loadedFaces = 0;
	for (unsigned int i = 0; i < 6; ++i) {
		const std::string& path = faces[i];	
		unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
		if (data)
		{
			GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			++loadedFaces;
		}
		else
		{
			DEBUG_LOG(2, ("Failed to load skybox texture: " + path).c_str());
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	skyboxReady = true;

	// Missing faces (e.g. textures not shipped in the public repo): draw a procedural sky instead.
	skyboxProcedural = (loadedFaces < 6);
	if (skyboxProcedural) {
		std::cerr << "INFO: Skybox textures not found, using procedural space sky." << std::endl;
	}
	stbi_set_flip_vertically_on_load(true);

}  

void RenderSystem::RenderSkybox()
{
	if (!skyboxReady || !skyboxShader || !skyboxMesh || !mainCameraComponent) {
		return;
	} 
	 
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_FRONT);

	skyboxShader->use();

	glm::mat4 viewNoTrans = glm::mat4(glm::mat3(viewMatrix));

	skyboxShader->SetMat4("uView", viewNoTrans);
	skyboxShader->SetMat4("uProjection", projectionMatrix);
	skyboxShader->SetInt("uSkybox", 0);
	skyboxShader->SetInt("uProcedural", skyboxProcedural ? 1 : 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap);

	skyboxMesh->Draw();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	 
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
}
 
void RenderSystem::RenderAll(float delta)
{
	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);      
	glDepthMask(GL_TRUE);

	if (sceneFramebuffer) {
		sceneFramebuffer->bind();
	}
	else {
		Framebuffer::BindDefault();
	}
	
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	RenderSkybox();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	auto& data = Engine::GetInstance().toolWindowData;
	if(data.isWaterScene && data.WireframeRendering) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
	}
	else {
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if(currentToolMode ==1) {
		defaultShader = colorByPositionShader;
	}
	else {
		defaultShader = lightingShader;
	}

	for (IRenderable* renderable : renderables) {
		if (!renderable) continue;
		renderable->Render(viewMatrix, projectionMatrix, delta);
	}
	 
	if (!sceneFramebuffer) {
		return;
	}

	// Post-Processing
	Framebuffer::BindDefault();
	 
	Texture* sceneColorTex = &sceneFramebuffer->colorTexture();
	Texture* sceneDepthTex = sceneFramebuffer->depthTexture();  
	 
	float camNear = mainCameraComponent->GetCamera().GetNearPlane();
	float camFar = mainCameraComponent->GetCamera().GetFarPlane();

	glDisable(GL_DEPTH_TEST);
	PostProcessManager::GetInstance().Apply(
		sceneColorTex,
		sceneDepthTex,
		/*outToScreen=*/true,
		camNear,
		camFar,
		Time::GetInstance().TotalTime()
	);
}
