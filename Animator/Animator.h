#ifndef _RGP_ANIMATOR_MASTER_H_
#define _RGP_ANIMATOR_MASTER_H_

#include ".//DataType.h"
#include "..//Core//Scene//GameScene.h"
#include "..//Core//Graphics//GLRenderer.h"
#include "..//Core//Graphics//Model3D.h"
#include "..//Core//Common//Timer.h"
#include "..//Core//Scene//DirectionnalLight.h"
#include "..//tools//imgui//imgui.h"

#define SKELETONEDITING 0
#define WEIGHMAPPING 1
#define ANIMATIONEDITING 2


using namespace RGP_CORE;

namespace RGP_ANIMATOR
{
	typedef struct {
		Model3D*		model;
		Skeleton*		skeleton;
		Animation*		animations;
		_u32b			numAnimations;
	} AnimationData;
	class Animator {
	public:
		static Animator* CreateAnimator();
		_bool		Init();
		_bool		Init_RGP_Sys();
		_bool		Init_ImGui_Sys();
	private:
		Animator();
		//UI part
		void		RenderUI();
		void		AnimationEditor();
		void		ModeSelector();
		void		BoneTool();
		void		WeightDrawingTool();
		void		AnimationPlayer();
		void		KeyEditor();
		//
	public:
		~Animator();
		void		Start();
		void		Destroy();

	private:
		
		GLRenderer *			m_SceneRenderer;
		GameScene*				m_Scene;
		Camera*					m_Camera;
		Timer*					m_Timer;
		_bool					m_Initialized;
		AnimationData*			m_Data;
		_u32b					m_NumData;
		_u8b					m_Mode;
	};

}

IMGUI_API bool        ImGui_ImplGlfwGL3_Init(GLFWwindow* window, bool install_callbacks);
IMGUI_API void        ImGui_ImplGlfwGL3_Shutdown();
IMGUI_API void        ImGui_ImplGlfwGL3_NewFrame();
IMGUI_API void        ImGui_ImplGlfwGL3_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplGlfwGL3_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplGlfwGL3_CreateDeviceObjects();

// GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
// Provided here if you want to chain callbacks.
// You can also handle inputs yourself and use those as a reference.
IMGUI_API void        ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
IMGUI_API void        ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
IMGUI_API void        ImGui_ImplGlfw_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
IMGUI_API void        ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c);

#endif

