#include ".//Animator.h"
#include "..//tools//glfw//glfw3.h"
#ifdef _WIN32	
#define	GLFW_EXPOSE_NATIVE_WIN32
	#include "..//tools//glfw//glfw3native.h"
#endif

//Animatir part

RGP_ANIMATOR::Animator* RGP_ANIMATOR::Animator::CreateAnimator()
{
	Animator* a = NULL;
	a = new Animator();
	if (!a)
		return NULL;
	if(!(a->Init())) {
		delete a;
		return NULL;
	}
	return a;
};

RGP_ANIMATOR::Animator::Animator() : m_SceneRenderer(NULL), m_Scene(NULL),
										m_Camera1(NULL), m_Camera2(NULL), m_Timer(NULL), m_Initialized(false),
										m_DynamicModels(NULL),m_NumDynamicModels(0), m_Mode(2), m_SelectedMesh(0),
										m_SelectedBone(0), m_SelectedAnimation(0)
{
};
RGP_ANIMATOR::Animator::~Animator()
{
	this->Destroy();
};
void RGP_ANIMATOR::Animator::Destroy()
{
	if (m_Scene) {
		m_Scene->Destroy();
		delete m_Scene;
		m_Scene = NULL;
	}
	//ImGui should be terminated before the renderer
	

	if (m_SceneRenderer) {
		m_SceneRenderer->Destroy();
		delete m_SceneRenderer;
		m_SceneRenderer = NULL;
	}
	if (m_DynamicModels) {
		free(m_DynamicModels);
		m_DynamicModels = NULL;
		m_NumDynamicModels = 0;
	}
	if (m_Timer) {
		delete m_Timer;
		m_Timer = NULL;
	}
	if (m_Camera1) {
		delete m_Camera1;
		m_Camera1 = NULL;
	}
	if (m_Camera2) {
		delete m_Camera2;
		m_Camera2 = NULL;
	}
	m_Initialized = false;
};
_bool RGP_ANIMATOR::Animator::Init()
{
	if (m_Initialized)
		return true;
	if (!Init_RGP_Sys())
		return false;
	if (!Init_ImGui_Sys())
		return false;
	m_Initialized = true;
	return true;
};
_bool RGP_ANIMATOR::Animator::Init_RGP_Sys()
{
	m_Scene = new GameScene();
	if (!m_Scene)
		return false;
	m_SceneRenderer = new GLRenderer();
	if (!m_SceneRenderer->InitRenderer({ "Animator",1200,900,false ,1024,false })) {
		return false;
	}
	m_Camera1 = new PerspCamera(M_PI_2, 1200.0f / 900.0f, 0.1f, 5000.0f);
	m_Camera2 = new OrthoCamera(-20.0f,20.0f,20.0f,-20.0f, 1.0f, 5000.0f);
	m_Camera1->setPosition({ 0.0f,12.0f,-10.0f });
	m_Camera1->setOrientation({ 0.0f, -0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f });
	m_Camera2->setPosition({ 0.0f,12.0f,-10.0f });
	m_Camera2->setOrientation({ 0.0f, -0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f });
	m_Scene->setCamera(m_Camera1);

	LightSource* light = NULL;
	light = new DirectionnalLight();
	light->setPosition({ 0.0f,12.0f,-10.0f });
	light->setOrientation({ 0.0f, -0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f });
	light->setLightSpecularColor({ 0.2f,0.2f,0.2f });

	m_Scene->AddActor(light);

	m_Timer = new Timer();

	//creating classes data base 
	Class_DB::addClass("Model3D", (void*(*)(void** args)) (&Model3D::Create));
	
	return true;
};
_bool RGP_ANIMATOR::Animator::Init_ImGui_Sys()
{
	

	return true;
};
void RGP_ANIMATOR::Animator::Start()
{
	if (!m_Initialized)
		return;

	this->ImportStaticModel("..//test//Samples//Plane.obj");
	//this->ImportDynamicModel("..//test//Samples//skeleton//skeleton.fbx");
	m_SceneRenderer->setScene(m_Scene);
	//m_SceneRenderer->SwitchNoLightMode();
	m_SceneRenderer->reRegisterLightSources();

	int state;
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	while (true) {
		glfwPollEvents();
		if (glfwWindowShouldClose(m_SceneRenderer->getTarget()->getglfwWindow()))
			break;
		ReactToEvents();
		//rendering part
		m_SceneRenderer->RenderCurrentScene();

		this->RenderUI();
	
		
		m_SceneRenderer->SwapBuffers();
	}
};
_bool		RGP_ANIMATOR::Animator::addAnimatedModel(AnimatedModel* model)
{
	AnimatedModel **tmp = (AnimatedModel**)malloc((m_NumDynamicModels + 1) * sizeof(AnimatedModel*));
	if (!tmp)
		return false;
	for (_u32b i = 0; i<m_NumDynamicModels; i++)
		tmp[i] = m_DynamicModels[i];
	free(m_DynamicModels);
	m_DynamicModels = tmp;
	m_DynamicModels[m_NumDynamicModels] = model;
	m_NumDynamicModels++;
	return true;
};

void RGP_ANIMATOR::Animator::RenderUI()
{
	this->AnimationEditor();
	this->ModeSelector();
	if (m_Mode == SKELETONEDITING) {
		BoneTool();
	}
	else if (m_Mode == WEIGHMAPPING) {
		WeightDrawingTool();
	}
	else if (m_Mode == ANIMATIONEDITING) {
		AnimationPlayer();
		KeyEditor();
	}
};

void RGP_ANIMATOR::Animator::ModeSelector()
{

};
void RGP_ANIMATOR::Animator::BoneTool()
{
	
};
void RGP_ANIMATOR::Animator::WeightDrawingTool()
{
	
};



void	 RGP_ANIMATOR::Animator::AnimationPlayer()
{
	
};
void	 RGP_ANIMATOR::Animator::AnimationEditor()
{
	
};
void	 RGP_ANIMATOR::Animator::KeyEditor()
{
	
};


_bool		RGP_ANIMATOR::Animator::ImportDynamicModel(_s8b* filename, Vertex3d Pos)
{
	AnimatedModel *model = NULL;
	if (!(model=AnimatedModel::CreateFromFile(m_SceneRenderer, filename)))
		return false;
	model->setPosition(Pos);
	m_Scene->AddActor(model);
	addAnimatedModel(model);
	return true;
};
#include "..//Core//ClassesDB//ClassesDB.h"
_bool		RGP_ANIMATOR::Animator::ImportStaticModel(_s8b* filename, Vertex3d Pos)
{
	/*Model3D* model = new Model3D();
	model->setRenderer(m_SceneRenderer);
	model->LoadModelFromFile(filename);*/
	BaseActor* (*ptr)(void**) = ((BaseActor*(*)(void** args))RGP_CORE::Class_DB::getCreateMethod("Model3D"));
	if (ptr) {
		BaseActor* model = NULL;
		void* args[2] = { m_SceneRenderer,filename };
		model = (*ptr)(args);
		model->setPosition(Pos);
		m_Scene->AddActor(model);
	}
	else {
		printf("null pointer \n");
	}
	return true;
};


void RGP_ANIMATOR::Animator::ReactToEvents()
{

	_double CursorPosx;
	_double CursorPosy;
	Vertex3d ver = { 0.0f,0.0f,0.0f };
	RGP_CORE::MyWindow* window = this->m_SceneRenderer->getTarget();
	//Mouse Events(+shift)
	if (glfwGetMouseButton(window->getglfwWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		glfwGetCursorPos(window->getglfwWindow(), &CursorPosx, &CursorPosy);
		if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS ||
			glfwGetKey(window->getglfwWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {//shift key is pressed
			ver.x = ((CursorPosx - m_CursorPos.u) / window->getWidth())* CONSTSTEP;
			ver.y = ((CursorPosy - m_CursorPos.v) / window->getHeight())* CONSTSTEP;
			m_Scene->getCamera()->TranslateViaSide(ver.x);
			m_Scene->getCamera()->TranslateViaUp(ver.y);

		}
		else if (glfwGetKey(window->getglfwWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS ||
			glfwGetKey(window->getglfwWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			ver.y = ((CursorPosy - m_CursorPos.v) / window->getHeight())* CONSTSTEP;
			m_Scene->getCamera()->TranslateViaDirection(ver.y*(1.0f));

		}
		else {
			ver.x = ((CursorPosx - m_CursorPos.u) / window->getWidth());
			ver.y = ((CursorPosy - m_CursorPos.v) / window->getHeight());
			m_Scene->getCamera()->RotateViaUp(ver.x*(-0.2f));
			m_Scene->getCamera()->RotateViaSide(ver.y*0.2f);
		}
	}
	//else {
	glfwGetCursorPos(window->getglfwWindow(), &CursorPosx, &CursorPosy);
	m_CursorPos.u = (_float)CursorPosx;
	m_CursorPos.v = (_float)CursorPosy;
	//}
	//keyboard shortcuts 
};
