#ifndef _RGP_ANIMATOR_MASTER_H_
#define _RGP_ANIMATOR_MASTER_H_

#include "..//Core//Scene//GameScene.h"
#include "..//Core//Graphics//GLRenderer.h"
#include "..//Core//Graphics//Model3D.h"
#include "..//Core//Common//Timer.h"
#include "..//Core//Scene//DirectionnalLight.h"
#include "..//tools//imgui//imgui.h"


using namespace RGP_CORE;

namespace RGP_ANIMATOR
{
	class Animator {
	public:
		static Animator* CreateAnimator();
		_bool		Init();
		_bool		Init_RGP_Sys();
		_bool		Init_ImGui_Sys();
	private:
		Animator();
		void		AnimationPlayer();
		void		AnimationEditor();
		void		AnimationKeyEditor();
	
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
		_bool					m_View;
		GLuint					m_FontTexture;
		//some other attributs will be added later 
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

