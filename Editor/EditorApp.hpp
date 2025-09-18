#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_stdlib.h>

#ifdef _WIN32
#undef APIENTRY // Thanks GLFW!! Shutting up the compiler
#include <Windows.h>
#endif

#include "Renderer/Renderer3D.hpp"
#include "Renderer/Viewport.hpp"
#include "Components/MeshRenderer.hpp"
#include "Components/Camera.hpp"

constexpr auto rad = 0.01745329251994329576923690768489f;

class EditorApp {
	public:
		EditorApp() {};
		void Run();

	private:
		void Init();
		void Start();
		void CleanUp();

		void InitScene();

		void renderObjectHierarchy(Node* object);
		void renderProjectFolderHierarchy(const std::filesystem::path& path);

		void CameraMoveControls();
		void NodeMenu();

		void GuiRenderMenuBar();
		void GuiRenderPopupWindows();
		void GuiRenderHierarchyWindow();
		void GuiRenderInspectorWindow();
		void GuiRenderProjectWindow();
		void GuiRenderSceneViewport();
		void GuiRenderGameViewport();
		void GuiRenderDebugWindow();

		GLFWwindow* window = nullptr;

		bool playing = false;

		Scene* scene = nullptr;
		Viewport* sceneViewport = nullptr;
		Viewport* gameViewport = nullptr;

		Node* selectedObject = nullptr;
		bool isEatingMouse = false;
		double mouseLockedPosX = 0.0, mouseLockedPosY = 0.0;

		std::string projectCreationWindowProjectsFolderSelectionText = "";
		std::string projectCreationWindowNewProjectNameInputText = "";

		std::filesystem::path projectFolder = "";
		std::filesystem::path projectFolderCurrentlySelected = "";
		bool showingInvalidFolderMessage = false;

		float lastTime = 0.0;
		float deltaTime = 0.0;
};