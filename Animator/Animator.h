#ifndef _RGP_ANIMATOR_MASTER_H_
#define _RGP_ANIMATOR_MASTER_H_

#include "..//Core//Common//Timer.h"
#include "..//Core//Graphics//GLRenderer.h"
#include "..//Core//AdvancedActors//DataType.h"
#include "..//Core//Scene//GameScene.h"
#include "..//Core//Scene//DirectionnalLight.h"
#include "..//Core//Scene//PointLight.h"
#include "..//tools//imgui//imgui.h"
#include ".//AnimatedModel.h"
#include "..//Core//ClassesDB//ClassesDB.h"

#define SKELETONEDITING 0
#define WEIGHMAPPING 1
#define ANIMATIONEDITING 2


using namespace RGP_CORE;

static _float			CONSTSTEP = 15.0f;
static Vertex3d			InsertionOffset = { 0.0f,0.0f,20.0f };

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
		_bool		addAnimatedModel(AnimatedModel* model);
		_bool		removeAnimatedModel(_u32b index);
		//UI part
		void		RenderUI();
		void		AnimationEditor();
		void		ModeSelector();
		void		BoneTool();
		void		WeightDrawingTool();
		void		AnimationPlayer();
		void		KeyEditor();

		//callback
		void				ReactToEvents();
		
		//
	public:
		~Animator();
		void		Start();
		void		Destroy();
		_bool		ImportDynamicModel(_s8b* filename, Vertex3d Pos = { 0.0,0.0,0.0 });
		_bool		ImportStaticModel(_s8b* filename, Vertex3d Pos = { 0.0,0.0,0.0 });


	private:
		
		GLRenderer *			m_SceneRenderer;
		GameScene*				m_Scene;
		Camera*					m_Camera1;
		Camera*					m_Camera2;
		Timer*					m_Timer;
		_bool					m_Initialized;
		AnimatedModel**			m_DynamicModels;
		_u32b					m_NumDynamicModels;
		_s32b					m_Mode;
		_u32b					m_SelectedMesh;//1 ---->NumMeshes
		_u32b					m_SelectedBone;//1 ---->Numbones
		_u32b					m_SelectedAnimation;//1 ---->NumAnimations
		Vertex2d				m_CursorPos;
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

