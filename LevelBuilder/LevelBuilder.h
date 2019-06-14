#ifndef _RGP_LEVEL_BUILDER_TOOL_H_
#define _RGP_LEVEL_BUILDER_TOOL_H_

#include "..//Core//Graphics//GLRenderer.h"
#include "..//Core//Physics//PhysicsEngine.h"
#include "..//Core//Sound//SoundEngine.h"
#include "..//Core//Common//Thread.h"
#include "CustomGameScene.h"
#include "Grid.h"



using namespace RGP_CORE;



static _float			CONSTSTEP = 15.0f;
static Vertex3d			InsertionOffset = { 0.0f,0.0f,20.0f };



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
		_bool				InitUI();
	public:
		_bool				isInitialized();
		BaseActor*			addActor(_s8b* Actortype,_s8b* instanceName);
		BaseActor*			getActorByIndex(_u32b );
		void				removeActor(BaseActor* ptrActor);
		void				removeActor(_u32b index);
		_bool				LoadDefaultScene();
		_bool				ImportScene(const _s8b* filename);
		_bool				ExportScene(const _s8b* filename);
		_bool				ImportStaticModel(const _s8b* filename);
		_bool				ImportDynamicModel(const _s8b* filename);

		void				TranslateSelected(Vertex3d translation);
		void				RotateSelected(Vertex3d rotation);//Quat 

		//Ops on Physicals type

		//Ops on light type 


		

	private :
		
		void				ReactToEvents();
		
	private :
		_bool					m_isInitilized;
		GLRenderer*				m_RendererInstance;
		PhysicsEngine*			m_PhysicsEngineInstance;
		SoundEngine*			m_SoundEngineInstance;
		CustomScene*			m_SceneInstance;
		Camera*					m_Camera;
		Thread*					m_RenderThread;
		//nanogui::Screen*		m_ScreenforUI;
		
		Vertex2d				m_CursorPos;
		EditCommands			m_CurrentCommand;
		TransformConstraint		m_SelectedAxis;
		
		FileNameUse				m_FilenameUse;
		/*TODO: Class Database provier*/
	};

}

extern _bool InitClassesDatabase();



#endif // !_RGP_LEVEL_BUILDER_TOOL_H_

