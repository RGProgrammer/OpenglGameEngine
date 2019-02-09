#ifndef _RGP_LEVEL_BUILDER_TOOL_H_
#define _RGP_LEVEL_BUILDER_TOOL_H_

#include "..//Core//Graphics//GLRenderer.h"
#include "..//Core//Physics//PhysicsEngine.h"
#include "..//Core//Sound//SoundEngine.h"
#include "..//Core//Common//Thread.h"
#include "..//tools//imgui//imgui.h"
#include "CustomGameScene.h"
using namespace RGP_CORE;


//
static _float			CONSTSTEP = 15.0f;
static Vertex3d			InsertionOffset = { 0.0f,0.0f,20.0f };



//IMGUI part
// GLFW data
static GLFWwindow*  g_Window = NULL;
static double       g_Time = 0.0f;
static bool         g_MouseJustPressed[3] = { false, false, false };
static GLFWcursor*  g_MouseCursors[ImGuiMouseCursor_COUNT] = { 0 };

// OpenGL3 data
static GLuint       g_FontTexture = 0;
static int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;



namespace RGP_LEVELBUILDER {
	class LevelBuilder {
	private:
		static const _u32b			MAXSELECTION=20;
		enum EditCommands {
			COMMAND_NONE=0,
			COMMAND_GRAB,
			COMMAND_SCALE,
			COMMAND_ROTATE,
			COMMAND_FILESELECT
		};
		enum TransformConstraint {
			TRANSFORM_ON_ALL = 0,
			TRANSFORM_ON_X,
			TRANSFORM_ON_Y,
			TRANSFORM_ON_Z
		};

		enum FileNameUse {
			USE_NOTHING = 0,
			USE_IMPORT_STATIC,//No animation
			USE_IMPORT_DYNAMIC,// with animation
			USE_IMPORT_SCENE,
			USE_EXPORT_SCENE
		};

	public:
		static LevelBuilder* Create();
		static void			Release();

		void StartLoop();

	private:
		
		LevelBuilder();
		~LevelBuilder();
		void				Destroy();
		_bool				Init();
	public:
		_bool				isInitialized();
		BaseActor*			addActor(_s8b* Actortype,_s8b* instanceName);
		BaseActor*			getActorByIndex(_u32b );
		void				removeActor(BaseActor* ptrActor);
		void				removeActor(_u32b index);
		//TODO
		_bool				LoadDefaultScene();
		_bool				ImportScene(const _s8b* filename);
		_bool				ExportScene(const _s8b* filename);
		_bool				ImportStaticModel(const _s8b* filename);
		_bool				ImportDynamicModel(const _s8b* filename);

		void				TranslateSelected(Vertex3d translation);
		void				RotateSelected(Vertex3d rotation);//Quat 

		//Ops on Physicals type

		//Ops on light type 


		//UI
		void				ActorsListingToolBox();
		void				BaseActorToolBox();
		void				SelectFileBox();
		void				LightingToolBox();
		void				DynamicActorToolBox();

	private :
		//
		void				ReactToEvents();
		
	private :
		_bool					m_isInitilized;
		GLRenderer*				m_RendererInstance;
		PhysicsEngine*			m_PhysicsEngineInstance;
		SoundEngine*			m_SoundEngineInstance;
		CustomScene*			m_SceneInstance;
		Camera*					m_Camera;
		Thread*					m_RenderThread;

		
		Vertex2d				m_CursorPos;
		EditCommands			m_CurrentCommand;
		TransformConstraint		m_SelectedAxis;
		_float					m_SelectedPosition[3];
		_float					m_SelectedRotation[3];
		_float					m_SelectedScale[3];
		FileNameUse				m_FilenameUse;
		/*TODO: Class Database provier*/
	};

}

extern _bool InitClassesDatabase();



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


#endif // !_RGP_LEVEL_BUILDER_TOOL_H_

