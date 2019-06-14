#include ".//LevelBuilder.h"
#ifdef _WIN32	
#define	GLFW_EXPOSE_NATIVE_WIN32
#include "..//tools//glfw//glfw3native.h"
#endif



_s8b Title[] = "LEVEL DEIGN TOOL";
static RGP_LEVELBUILDER::LevelBuilder* _Instance=NULL ;
RGP_LEVELBUILDER::LevelBuilder* RGP_LEVELBUILDER::LevelBuilder::Create()
{
	if (_Instance)
		return _Instance;
	_Instance = new LevelBuilder();
	if (!_Instance)
		return NULL;
	if (!_Instance->Init()) {
		delete _Instance;
		_Instance = NULL;
		return NULL;
	}
	_Instance->InitUI();
	return _Instance;
};
void		RGP_LEVELBUILDER::LevelBuilder::Release()
{
	if (_Instance) {
		delete _Instance;
		_Instance = NULL; 
	}
};

void RGP_LEVELBUILDER::LevelBuilder::StartLoop()
{
	if (!m_isInitilized)
		return;
	//m_RendererInstance->printExtension();
	m_RendererInstance->reRegisterLightSources();
	while (true) {
		glfwPollEvents();
		if (glfwWindowShouldClose(m_RendererInstance->getTarget()->getglfwWindow()))
			break;
		ReactToEvents();
		


		m_RendererInstance->RenderCurrentScene();
		//m_ScreenforUI->drawContents();
		//m_ScreenforUI->drawWidgets();
		
		m_RendererInstance->SwapBuffers();
	}
};

RGP_LEVELBUILDER::LevelBuilder::LevelBuilder():m_isInitilized(false),
												m_RendererInstance(NULL), m_PhysicsEngineInstance(NULL),
												m_SoundEngineInstance(NULL), m_SceneInstance(NULL),
												m_RenderThread(NULL), m_Camera(NULL),
												m_CurrentCommand(COMMAND_NONE), m_SelectedAxis(TRANSFORM_ON_ALL),
												m_FilenameUse(USE_NOTHING)//, m_ScreenforUI(NULL)
{

};
RGP_LEVELBUILDER::LevelBuilder::~LevelBuilder()
{
	this->Destroy();
};
void				RGP_LEVELBUILDER::LevelBuilder::Destroy()
{
	m_isInitilized = false;
	m_RendererInstance=NULL ;
	m_PhysicsEngineInstance=NULL ;
	m_SoundEngineInstance=NULL ; 
	
	if (m_SceneInstance) {
		delete m_SceneInstance;
		m_SceneInstance = NULL;
	}
	if (m_RenderThread) {
		delete m_RenderThread;
		m_RenderThread = NULL;
	}
	if (m_Camera) {
		delete m_Camera;
		m_Camera = NULL;
	}
	/*if (m_ScreenforUI) {
		delete m_ScreenforUI;
		m_ScreenforUI = NULL;
	}
	nanogui::shutdown();*/
	if (m_RendererInstance) {
		delete m_RendererInstance;
		m_RendererInstance = NULL;
	}
};
_bool				RGP_LEVELBUILDER::LevelBuilder::Init()
{
	if (!InitClassesDatabase()) {
		printf("error init classes database \n");
		return false;
	}
	m_SceneInstance = new CustomScene();
	if (!m_SceneInstance) {
		this->Destroy();
		return false;
	}
	m_Camera = new PerspCamera(M_PI_2, 1600.0f / 900.0f, 0.1f, 5000.0f);
	m_SceneInstance->setCamera(m_Camera);
	m_Camera->setPosition({ 0.0f,12.0f,-10.0f });
	m_Camera->setOrientation({ 0.0f, -0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f });
	m_RendererInstance = new RGP_CORE::GLRenderer();
	if (!m_RendererInstance->InitRenderer({ Title,1600,900,false ,256,false })) {
		return false;
	}
	m_RendererInstance->setScene(m_SceneInstance);

	m_PhysicsEngineInstance = new PhysicsEngine();
	m_PhysicsEngineInstance->Init(m_SceneInstance);
	InitClassesDatabase();
	LoadDefaultScene();

	m_isInitilized = true;

	return true;
}
/*
//for test
bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";
nanogui::FormHelper *gui=NULL;*/
//
_bool RGP_LEVELBUILDER::LevelBuilder::InitUI()
{
	/*nanogui::init();
	printf("done init NANOGUI\n");
	m_ScreenforUI = new nanogui::Screen();
	m_ScreenforUI->initialize(m_RendererInstance->getTarget()->getglfwWindow(),false);
	gui = new 	nanogui::FormHelper(m_ScreenforUI);
	gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
	gui->addGroup("Basic types");
	gui->addVariable("bool", bvar)->setTooltip("Test tooltip.");
	gui->addVariable("string", strval);

	gui->addGroup("Validating fields");
	gui->addVariable("int", ivar)->setSpinnable(true);
	gui->addVariable("float", fvar)->setTooltip("Test.");
	gui->addVariable("double", dvar)->setSpinnable(true);*/

	return true;
}
;

_bool				RGP_LEVELBUILDER::LevelBuilder::isInitialized() { return m_isInitilized; };
BaseActor*			RGP_LEVELBUILDER::LevelBuilder::addActor(_s8b* Actortype, _s8b* instanceName) 
{
	BaseActor* instance = NULL;
	_u32b index =
	RGP_CORE::Class_DB::find(Actortype);
	if (!index)
		return false;
	BaseActor* (*ptr)(void**) = ((BaseActor*(*)(void** args))
		RGP_CORE::Class_DB::m_Classes[index - 1].createInstanceMethod) ;
	instance = (*ptr)(NULL);
	m_SceneInstance->AddActor(instance);
	return instance;
};

BaseActor*			RGP_LEVELBUILDER::LevelBuilder::getActorByIndex(_u32b Actorindex)
{
	if (m_SceneInstance)
		return m_SceneInstance->getActor(Actorindex);
	else
		return NULL;
};
void				RGP_LEVELBUILDER::LevelBuilder::removeActor(BaseActor* ptrActor)
{
	if (m_SceneInstance) {
		m_SceneInstance->RemoveActor(ptrActor);
	}
};
void				RGP_LEVELBUILDER::LevelBuilder::removeActor(_u32b index)
{
	if (m_SceneInstance)
		m_SceneInstance->RemoveActorAt(index);
};


_bool				RGP_LEVELBUILDER::LevelBuilder::LoadDefaultScene()
{
	BaseActor* Actor = NULL;

	BaseActor* (*ptr)(void**) = NULL;
	
	Actor =Grid::CreateGrid(m_RendererInstance);
	Actor->setName("Grid");
	m_SceneInstance->AddActor(Actor);
	
	

	ptr = ((BaseActor*(*)(void** args))RGP_CORE::Class_DB::getCreateMethod("DirectionnalLight"));
	if (ptr) {
		Actor = NULL;
		Actor = (*ptr)(NULL);
		if (Actor) {
			Actor->setName("light");
			m_SceneInstance->AddActor(Actor);
		}
	}
	else
		return false;

	
	
	return true;

};
_bool				RGP_LEVELBUILDER::LevelBuilder::ImportScene(const _s8b* filename)
{

	return false;

};
_bool				RGP_LEVELBUILDER::LevelBuilder::ExportScene(const _s8b* filename)
{
	return false;
};
_bool				RGP_LEVELBUILDER::LevelBuilder::ImportStaticModel(const _s8b* filename)
{
	BaseActor* (*ptr)(void**) = ((BaseActor*(*)(void** args))RGP_CORE::Class_DB::getCreateMethod("Model3D"));
	if (ptr) {
		void** args =(void**) malloc(2 * sizeof(void*));
		args[0] = m_RendererInstance;
		args[1] = (void*)filename;
		BaseActor* model = NULL;
		model = (*ptr)(args);
		m_SceneInstance->AddActor(model);
	}
	else {
		printf("null pointer \n");
	}
	return true;
};
_bool				RGP_LEVELBUILDER::LevelBuilder::ImportDynamicModel(const _s8b* filename)
{
	return false;
};

void				RGP_LEVELBUILDER::LevelBuilder::TranslateSelected(Vertex3d translation)
{
	for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
		if(m_SceneInstance->isActorSelected(i))
			m_SceneInstance->getActor(i)->Translate(translation);
	}
};
void				RGP_LEVELBUILDER::LevelBuilder::RotateSelected(Vertex3d rotation)
{
	for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
		if (m_SceneInstance->isActorSelected(i)){
			m_SceneInstance->getActor(i)->RotateViaDirection(rotation.z);
			m_SceneInstance->getActor(i)->RotateViaUp(rotation.y);
			m_SceneInstance->getActor(i)->RotateViaSide(rotation.x);
		}
	}
}; 



void RGP_LEVELBUILDER::LevelBuilder::ReactToEvents()
{
	
	_double CursorPosx; 
	_double CursorPosy;
	Vertex3d ver = { 0.0f,0.0f,0.0f };
	Vertex3d transformHelper;
	RGP_CORE::MyWindow* window= this->m_RendererInstance->getTarget();
	glfwGetCursorPos(window->getglfwWindow(), &CursorPosx, &CursorPosy);
	ver.x = ((CursorPosx - m_CursorPos.u) / window->getWidth());
	ver.y = ((CursorPosy - m_CursorPos.v) / window->getHeight());

	
	
	if (m_CurrentCommand == COMMAND_NONE) {//if no command 
		//Mouse Events(+shift)
		if (glfwGetMouseButton(window->getglfwWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
			
			if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS ||
				glfwGetKey(window->getglfwWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {//shift key is pressed
				ver.x *= CONSTSTEP;
				ver.y *= CONSTSTEP;
				m_SceneInstance->getCamera()->TranslateViaSide(ver.x);
				m_SceneInstance->getCamera()->TranslateViaUp(ver.y);

			}
			else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS ||
				glfwGetKey(window->getglfwWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
				ver.y *= CONSTSTEP*3.0f;
				m_SceneInstance->getCamera()->TranslateViaDirection(ver.y*(1.0f));

			}
			else {
				m_SceneInstance->getCamera()->RotateViaUp(ver.x*(-0.2f));
				m_SceneInstance->getCamera()->RotateViaSide(ver.y*0.2f);
			}
		}

		//unselect all the selected
		if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_DELETE) == GLFW_PRESS)
			m_SceneInstance->UnselectAll();
		
		//Keyboard shortcuts
		if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_DELETE) == GLFW_PRESS) {
			m_SceneInstance->RemoveSelectedActors();

		}
		else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_G) == GLFW_PRESS) {
			m_CurrentCommand = COMMAND_GRAB;

		}
		else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_R) == GLFW_PRESS) {
			m_CurrentCommand = COMMAND_ROTATE;
		}
		else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_S) == GLFW_PRESS) {
			m_CurrentCommand = COMMAND_SCALE;
		}
		else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_I) == GLFW_PRESS &&
			(glfwGetKey(window->getglfwWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window->getglfwWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)) {
			m_CurrentCommand = COMMAND_FILESELECT;
			m_FilenameUse = USE_IMPORT_STATIC;
		}
	}
	else {
		if (glfwGetMouseButton(window->getglfwWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && m_CurrentCommand != COMMAND_FILESELECT) {
			m_CurrentCommand = COMMAND_NONE;
			m_SelectedAxis = TRANSFORM_ON_ALL;
			m_FilenameUse = USE_NOTHING;
			return;
		}

		if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			m_CurrentCommand = COMMAND_NONE;
			m_SelectedAxis = TRANSFORM_ON_ALL;
			m_FilenameUse = USE_NOTHING;
			return;
		}
		else {
			if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_X) == GLFW_PRESS) {
				if (m_SelectedAxis != TRANSFORM_ON_X)
					m_SelectedAxis = TRANSFORM_ON_X;
				else
					m_SelectedAxis = TRANSFORM_ON_ALL;
			}
			if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_Y) == GLFW_PRESS) {
				if (m_SelectedAxis != TRANSFORM_ON_Y)
					m_SelectedAxis = TRANSFORM_ON_Y;
				else
					m_SelectedAxis = TRANSFORM_ON_ALL;
			}
			if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_Z) == GLFW_PRESS) {
				if (m_SelectedAxis != TRANSFORM_ON_Z)
					m_SelectedAxis = TRANSFORM_ON_Z;
				else
					m_SelectedAxis = TRANSFORM_ON_ALL;
			}
		}
	}
	
	
	if(m_CurrentCommand==COMMAND_GRAB){// if current command Grab(translate)
		
		//grab Command reaction
		ver.x *= CONSTSTEP;
		ver.y *= CONSTSTEP;
		if (m_SelectedAxis == TRANSFORM_ON_ALL) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = ScaleVertex3d(m_SceneInstance->getCamera()->getSide(), -ver.x);
					transformHelper = AddVertex3d(transformHelper, ScaleVertex3d(m_SceneInstance->getCamera()->getUp(), -ver.y));
					m_SceneInstance->getActor(i)->Translate(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_X) {

			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = ScaleVertex3d(m_SceneInstance->getActor(i)->getSide(), ver.x);
					m_SceneInstance->getActor(i)->Translate(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Y) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = ScaleVertex3d(m_SceneInstance->getActor(i)->getUp(), -ver.y);
					m_SceneInstance->getActor(i)->Translate(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Z) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = ScaleVertex3d(m_SceneInstance->getActor(i)->getDirection(), ver.x);
					m_SceneInstance->getActor(i)->Translate(transformHelper);
				}
			}
		}
	}
	else if (m_CurrentCommand == COMMAND_SCALE) {
		//SCale command reaction
		if (m_SelectedAxis == TRANSFORM_ON_ALL) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					m_SceneInstance->getActor(i)->ScaleUniformAdd(ver.y);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_X) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = m_SceneInstance->getActor(i)->getScale();
					transformHelper.x += ver.x;
					m_SceneInstance->getActor(i)->Scale(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Y) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = m_SceneInstance->getActor(i)->getScale();
					transformHelper.y += ver.y;
					m_SceneInstance->getActor(i)->Scale(transformHelper);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Z) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					transformHelper = m_SceneInstance->getActor(i)->getScale();
					transformHelper.z += ver.x;
					m_SceneInstance->getActor(i)->Scale(transformHelper);
				}
			}
		}
	}
	else if (m_CurrentCommand == COMMAND_ROTATE) {
		//Rotation Command Reaction
		 if (m_SelectedAxis == TRANSFORM_ON_X) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					m_SceneInstance->getActor(i)->RotateViaSide(ver.x);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Y) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					m_SceneInstance->getActor(i)->RotateViaUp(ver.y);
				}
			}
		}
		else if (m_SelectedAxis == TRANSFORM_ON_Z) {
			for (_u32b i = 0; i < m_SceneInstance->getNumActors(); ++i) {
				if (m_SceneInstance->isActorSelected(i)) {
					m_SceneInstance->getActor(i)->RotateViaDirection(ver.x);
				}
			}
		}

	}
	
	glfwGetCursorPos(window->getglfwWindow(), &CursorPosx, &CursorPosy);
	m_CursorPos.u = (_float)CursorPosx;
	m_CursorPos.v = (_float)CursorPosy;
	
};


