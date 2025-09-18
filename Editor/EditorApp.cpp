#include "EditorApp.hpp"

void EditorApp::Run() {
	Init();
	Start();
	CleanUp();
}

void EditorApp::Init() {
	if (!glfwInit()) {
		return;
	}
	window = glfwCreateWindow(800, 600, "Unnamed Engine 0.0.0", nullptr, nullptr);
	if (window == nullptr)
		return;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable VSync

	if (glewInit() != GLEW_OK) {
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");

	#ifdef _WIN32
		// Account for the DPI Scale setting on Windows
		float dpiScale = GetDpiForSystem() / 96.f;
	#else
		float dpiScale = 1.0f;
	#endif

	style.ScaleAllSizes(dpiScale);

	io.Fonts->AddFontFromFileTTF("Roboto.ttf", 14.f * dpiScale);
	ImFontConfig iconFontConfig;
	iconFontConfig.MergeMode = true;
	io.Fonts->AddFontFromFileTTF("FontAwesome.otf", 12.f * dpiScale, &iconFontConfig);

	lastTime = (float)glfwGetTime();

	InitScene();
}

void EditorApp::InitScene() {
	// Scene Setup

	scene = new Scene();

	Node* rootNode = new Node("Game", scene);

	scene->rootNode = rootNode;

	Node* theobject = new Node("Awesome cube!!", scene->rootNode);
	theobject->AddComponent(new MeshRenderer(new Mesh("./Portal2Cube.glb"), new Material("./Metal_box.png")));
	theobject->transform.position = Vector3(0, 0, 5);

	Node* theobject2 = new Node("Epic cube", scene->rootNode);
	theobject2->transform.position = Vector3(0, 0, -5);

	Node* camera = new Node("Main Camera", scene->rootNode);
	Camera* gameCameracomp = new Camera();
	camera->AddComponent(gameCameracomp);
	camera->transform.position = Vector3(0, 0, -3);

	// Viewports Setups

	Node* sceneCamera = new Node("Scene Camera (Non-Existant)");
	Camera* sceneCameracomp = new Camera();
	sceneCamera->AddComponent(sceneCameracomp);
	sceneCamera->transform.position = Vector3(0, 0, -3);
	sceneCamera->parent = scene->rootNode; // One way scene->RootNode reference, scene->RootNode doesn't know sceneCamera but sceneCamera knows scene->RootNode. This is a bad and ugly and messy solution but it works.

	scene->mainCamera = gameCameracomp;

	//scene->SaveScene("./EpicScene.jscene");

	//scene->LoadScene("./EpicScene.jscene");

	// Editor
	Renderer3D* renderer = new Renderer3D();
	renderer->overrideCamera = sceneCameracomp;
	renderer->scene = scene;

	sceneViewport = new Viewport();
	sceneViewport->renderer = renderer;

	// Game
	Renderer3D* gameRenderer = new Renderer3D();
	gameRenderer->scene = scene;

	gameViewport = new Viewport();
	gameViewport->renderer = gameRenderer;
}

void EditorApp::Start() {
	while (!glfwWindowShouldClose(window)) {
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;

		glfwPollEvents();

		// GUI
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

			GuiRenderMenuBar();

			GuiRenderPopupWindows();
			GuiRenderHierarchyWindow();
			GuiRenderInspectorWindow();
			GuiRenderProjectWindow();

			GuiRenderSceneViewport();
			GuiRenderGameViewport();

			GuiRenderDebugWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		lastTime = currentTime;
	}
}

void EditorApp::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void EditorApp::NodeMenu() {
	if (ImGui::MenuItem("New Empty")) {
		Node* newObject = new Node("New Node", selectedObject ? selectedObject : scene->rootNode);
		selectedObject = newObject;
	}
	if (ImGui::MenuItem("New Cube")) {
		Node* newObject = new Node("New Cube", selectedObject ? selectedObject : scene->rootNode);
		newObject->AddComponent(new MeshRenderer(new Mesh("./Cube.obj"), new Material()));
		selectedObject = newObject;
	}
	if (selectedObject) {
		ImGui::Separator();
		if (ImGui::MenuItem("Delete")) {
			selectedObject->Destroy();
			selectedObject = nullptr;
		}
	}
}

void EditorApp::GuiRenderMenuBar() {
	bool shouldOpenTheThing = false;
	bool shouldOpenTheThing2 = false;
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New", "Ctrl+N")) {
				shouldOpenTheThing = true;
			}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {
				shouldOpenTheThing2 = true;
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			if (ImGui::MenuItem("Eat toilets")) {
				abort();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Node")) {
			NodeMenu();
			ImGui::EndMenu();
		}

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 32.f) * 0.5f);
		if (ImGui::Button(playing ? "" : "", {32.f, 0.0f})) {
			if (playing) {
				playing = false;
				ImGui::SetWindowFocus("Scene");
			} else {
				playing = true;
				ImGui::SetWindowFocus("Game");
			}
		}
		//ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, 0 }, { ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowHeight() }, 0xFFFFFFFF, 4.0f);

		ImGui::EndMainMenuBar();
	}
	if (shouldOpenTheThing) ImGui::OpenPopup("Create new project");
	if (shouldOpenTheThing2) ImGui::OpenPopup("Open existing project");
}

void EditorApp::GuiRenderPopupWindows() {
	if (ImGui::BeginPopupModal("Create new project", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Projects Folder:");
		ImGui::InputText("##ProjectsFolderSelector", &projectCreationWindowProjectsFolderSelectionText);
		ImGui::Text("New Project Name:");
		ImGui::InputText("##NewProjectNameInput", &projectCreationWindowNewProjectNameInputText);
		if (!projectCreationWindowProjectsFolderSelectionText.empty() && !std::filesystem::is_directory(projectCreationWindowProjectsFolderSelectionText)) ImGui::Text("(%s will be created)", std::filesystem::path(projectCreationWindowProjectsFolderSelectionText).filename().string().c_str());
		if (ImGui::Button("Create Project")) {
			std::filesystem::path projectFolderPath = std::filesystem::path(projectCreationWindowProjectsFolderSelectionText) / std::filesystem::path(projectCreationWindowNewProjectNameInputText);
			std::filesystem::create_directories(projectFolderPath);
			projectFolder = projectFolderPath;
			ImGui::CloseCurrentPopup();

			projectCreationWindowProjectsFolderSelectionText = "";
			projectCreationWindowNewProjectNameInputText = "";
			showingInvalidFolderMessage = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();

			projectCreationWindowProjectsFolderSelectionText = "";
			projectCreationWindowNewProjectNameInputText = "";
			showingInvalidFolderMessage = false;
		}

		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("Open existing project", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Project Folder:");
		ImGui::InputText("##ProjectsFolderSelector", &projectCreationWindowProjectsFolderSelectionText);
		if (showingInvalidFolderMessage) ImGui::Text("Invalid project folder!!!");
		if (ImGui::Button("Open Project")) {
			std::filesystem::path projectFolderPath(projectCreationWindowProjectsFolderSelectionText);
			if (std::filesystem::is_directory(projectFolderPath)) {
				projectFolder = projectFolderPath;
				ImGui::CloseCurrentPopup();

				projectCreationWindowProjectsFolderSelectionText = "";
				projectCreationWindowNewProjectNameInputText = "";
				showingInvalidFolderMessage = false;
			} else {
				showingInvalidFolderMessage = true;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();

			projectCreationWindowProjectsFolderSelectionText = "";
			projectCreationWindowNewProjectNameInputText = "";
			showingInvalidFolderMessage = false;
		}

		ImGui::EndPopup();
	}
}

void EditorApp::renderObjectHierarchy(Node* object) {
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (object->children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;
	if (selectedObject == object) flags |= ImGuiTreeNodeFlags_Selected;

	ImGui::PushID(object);
	bool treeNode = ImGui::TreeNodeEx(object->name.c_str(), flags);
	ImGui::PopID();

	if (ImGui::BeginPopupContextItem()) {
		NodeMenu();
		ImGui::EndPopup();
	}

	if (ImGui::IsItemClicked()) {
		selectedObject = object;
	}

	if (treeNode) {
		for (Node* child : object->children) {
			renderObjectHierarchy(child);
		}
		ImGui::TreePop();
	}
}

void EditorApp::GuiRenderHierarchyWindow() {
	if (ImGui::Begin("Hierarchy")) {
		if (scene) {
			renderObjectHierarchy(scene->rootNode);
		} else {
			ImGui::Text("No scene!!!!");
		}
	} ImGui::End();
}

void EditorApp::GuiRenderInspectorWindow() {
	if (ImGui::Begin("Inspector")) {
		if (selectedObject) {
			ImGui::InputText("##InputName", &selectedObject->name);
			ImGui::Separator();

			ImGui::Text("Transform");
			ImGui::DragFloat3("Position", &selectedObject->transform.position.x, 0.01f);
			ImGui::DragFloat3("Rotation", &selectedObject->transform.rotation.x, 1.0f);
			ImGui::DragFloat3("Scale", &selectedObject->transform.scale.x, 0.01f);
			ImGui::Separator();

			for (Component* component : selectedObject->components) {
				ComponentTypeInfo info = component->GetInfo();
				ImGui::Text(info.name.c_str());
				for (const ComponentFieldInfo& field : info.fields) {
					void* fieldptr = (char*)component + field.offset;
					ImGui::PushID(fieldptr);
					switch (field.type) {
						case InputType::Float:
							ImGui::DragFloat(field.name.c_str(), (float*)fieldptr, field.step);
							break;
						case InputType::Int:
							ImGui::DragInt(field.name.c_str(), (int*)fieldptr, field.step);
							break;
						case InputType::Bool:
							ImGui::Checkbox(field.name.c_str(), (bool*)fieldptr);
							break;
						case InputType::String:
							ImGui::InputText(field.name.c_str(), (std::string*)fieldptr);
							break;
						case InputType::Vector3:
							ImGui::DragFloat3(field.name.c_str(), &((Vector3*)fieldptr)->x, field.step);
							break;
						case InputType::Vector2:
							ImGui::DragFloat3(field.name.c_str(), &((Vector2*)fieldptr)->x, field.step);
							break;
					}
					ImGui::PopID();
				}
				ImGui::Separator();
			}

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("Add Component Popup");
			}

			if (ImGui::BeginPopup("Add Component Popup")) {
				for (auto& [name, factory] : ComponentRegistry::Get()) {
					if (ImGui::MenuItem(name.c_str())) {
						Component* c = factory();
						selectedObject->AddComponent(c);
					}
				}
				ImGui::EndPopup();
			}
		} else {
			ImGui::Text("Nothing Selected!");
		}
	} ImGui::End();
}

void EditorApp::renderProjectFolderHierarchy(const std::filesystem::path& path) {
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (entry.path() == projectFolderCurrentlySelected) flags |= ImGuiTreeNodeFlags_Selected;
		if (!entry.is_directory()) flags |= ImGuiTreeNodeFlags_Leaf;
		bool treeNode = ImGui::TreeNodeEx(entry.path().filename().string().c_str(), flags);
		if (ImGui::IsItemClicked()) {
			projectFolderCurrentlySelected = entry.path();
		}
		if (treeNode) {
			if (entry.is_directory()) renderProjectFolderHierarchy(entry.path());
			ImGui::TreePop();
		}
	}
}

void EditorApp::GuiRenderProjectWindow() {
	if (ImGui::Begin("Project")) {
		if (projectFolder.empty()) {
			ImGui::Text("No project!!! Create one using File>New or Ctrl+N");
		} else {
			renderProjectFolderHierarchy(projectFolder);
		}
	} ImGui::End();
}

void EditorApp::CameraMoveControls() {
	double mousePosX, mousePosY;
	glfwGetCursorPos(window, &mousePosX, &mousePosY);

	if (!isEatingMouse && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
		isEatingMouse = true;
		mouseLockedPosX = mousePosX;
		mouseLockedPosY = mousePosY;
	}

	if (isEatingMouse) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);

		glfwSetCursorPos(window, mouseLockedPosX, mouseLockedPosY);

		Transform& t = sceneViewport->renderer->overrideCamera->object->transform;
		t.rotation.x += ((float)mousePosY - (float)mouseLockedPosY) * 0.5f;
		t.rotation.y += ((float)mousePosX - (float)mouseLockedPosX) * 0.5f;
		Vector3 moveAngleZ = { sin(t.rotation.y * rad), -sin(t.rotation.x * rad), cos(t.rotation.y * rad) };
		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			t.position += moveAngleZ * 8.f * deltaTime;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			t.position += moveAngleZ * -8.f * deltaTime;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			t.position += Vector3::cross({ 0.0f, 1.0f, 0.0f }, moveAngleZ) * 8.f * deltaTime;
		}
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			t.position += Vector3::cross({ 0.0f, 1.0f, 0.0f }, moveAngleZ) * -8.f * deltaTime;
		}
	}
	if (isEatingMouse && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
		isEatingMouse = false;
	}
}

void EditorApp::GuiRenderSceneViewport() {
	if (ImGui::Begin("Scene")) {
		ImVec2 wsize = ImGui::GetContentRegionAvail();
		GLsizei wwidth = (GLsizei)wsize.x, wheight = (GLsizei)wsize.y;

		sceneViewport->ResizeFB(wwidth, wheight);

		sceneViewport->Render();

		ImGui::Image((ImTextureID)sceneViewport->texture, wsize, ImVec2(0, 1), ImVec2(1, 0));

		CameraMoveControls();
		
	} ImGui::End();
}

void EditorApp::GuiRenderGameViewport() {
	if (ImGui::Begin("Game")) {
		ImVec2 wsize = ImGui::GetContentRegionAvail();
		GLsizei wwidth = (GLsizei)wsize.x, wheight = (GLsizei)wsize.y;

		gameViewport->ResizeFB(wwidth, wheight);

		gameViewport->Render();

		ImGui::Image((ImTextureID)gameViewport->texture, wsize, ImVec2(0, 1), ImVec2(1, 0));
	} ImGui::End();
}

void EditorApp::GuiRenderDebugWindow() {
	if (ImGui::Begin("Debug")) {
		ImGui::Text("Frame Time: %f MS", deltaTime * 1000.f);
		ImGui::Text("FPS: %f", 1.f / deltaTime);
	} ImGui::End();
}