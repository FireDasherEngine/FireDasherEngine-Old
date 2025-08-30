#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

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
		void Init();
		void Start();

	private:
		void InitScene();

		void renderObjectHierarchy(Node* object);

		void CameraMoveControls();
		void NodeMenu();

		void GuiRenderMenuBar();
		void GuiRenderPopupWindows();
		void GuiRenderHierarchyWindow();
		void GuiRenderInspectorWindow();
		void GuiRenderProjectWindow();
		void GuiRenderSceneViewport();
		void GuiRenderGameViewport();

		GLFWwindow* window;

		bool playing;

		Scene* scene;
		Viewport* sceneViewport;
		Viewport* gameViewport;

		Node* selectedObject = nullptr;
		bool isShowingObjectRenamingWindow = false;
		std::string objectRenamingWindowInputText = "";
		bool isEatingMouse = false;
		double mouseLockedPosX, mouseLockedPosY;
};