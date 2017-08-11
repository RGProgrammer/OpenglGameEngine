#include "Test.h"

RGP_CORE::Test::Test():m_Renderer(NULL),m_CurrentScene(NULL),m_Camera(NULL),m_Physics(NULL),m_Timer(NULL)
{
};
RGP_CORE::Test::~Test(){
    this->Destroy() ;
};
void RGP_CORE::Test::Destroy(){
    if(m_CurrentScene){
        m_CurrentScene->Destroy();
        delete m_CurrentScene ;
        m_CurrentScene=NULL ;
    }
    if(m_Camera){
        delete m_Camera;
        m_Camera=NULL ;
    }
    if(m_Renderer){
        m_Renderer->Destroy();
        delete m_Renderer ;
        m_Renderer=NULL;
    }
	if (m_Physics) {
		delete m_Physics;
		m_Physics = NULL;
	}
	if (m_Timer) {
		delete m_Timer;
		m_Timer = NULL;
	}
};
void RGP_CORE::Test::Start() {

	RGP_CORE::Model3D* testmodel1 = NULL;
	LightSource* light = NULL;
	PModel*	PM = NULL;



	/*light=new DirectionnalLight();
	light->setOrientation({0.0f,-1.0f,0.0f}, {0.0f,0.0f,1.0f});
	m_CurrentScene->AddLight(light);*/
	for (int k = 0; k < 4; ++k) {
		light = new SpotLight();
		light->setPosition({ 0.0f,10.0f,0.0f- k*8.0f });
		light->setOrientation({ 0.0,-1.0f,1.0f }, { 0.0f,0.0f,1.0f });
		m_CurrentScene->AddLight(light);
	}


	PM = PModel::CreateGround(m_Renderer,{ 0.0f,-1.0f,0.0f });
	m_CurrentScene->AddActor(PM);
	
	PM = PModel::CreateCube(m_Renderer, { -8.0f,10.0f,0.0f }, {0.0f,0.0f,1.0f},{0.0f,1.0f,0.0f} );
	m_CurrentScene->AddActor(PM);

	PM = PModel::CreateSphere(m_Renderer, { 8.0f,10.0f,0.0f }, { 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f });
	m_CurrentScene->AddActor(PM);
	
	///ecerything is good
	m_Renderer->setScene(m_CurrentScene);
	m_Physics->setScene(m_CurrentScene);

	m_Timer->Reset();
    while(true){
        int state;
        glfwPollEvents();
        if(glfwWindowShouldClose(m_Renderer->getTarget()->getglfwWindow()))
            break ;


        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_W);
        if (state == GLFW_PRESS){
            m_Camera->Translate(ScaleVertex3d(m_Camera->getDirection(),2.0));
			
        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_S);
        if (state == GLFW_PRESS){
            m_Camera->Translate(ScaleVertex3d(m_Camera->getDirection(),-2.0));
        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_D);
        if (state == GLFW_PRESS){
            m_Camera->Translate(ScaleVertex3d(m_Camera->getSide(),-2.0));

        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_A);
        if (state == GLFW_PRESS){
            m_Camera->Translate(ScaleVertex3d(m_Camera->getSide(),2.0));

        }


        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_UP);
        if (state == GLFW_PRESS){
            m_Camera->RotateViaSide(-0.01);

        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_DOWN);
        if (state == GLFW_PRESS){
            m_Camera->RotateViaSide(0.01);

        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_LEFT);
        if (state == GLFW_PRESS){
            m_Camera->RotateViaUp(0.01);

        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_RIGHT);
        if (state == GLFW_PRESS){
            m_Camera->RotateViaUp(-0.01);
        }
		state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_B);
		if (state == GLFW_PRESS) {
			PM = PModel::CreateCube(m_Renderer,AddVertex3d(m_Camera->getPosition(), ScaleVertex3d(m_Camera->getDirection(), 5.0f)), {0.0f,0.0f,1.0f}, {0.0f,1.0f,0.0f});
			if (PM) {
				PM->setRenderer(m_Renderer);
				m_CurrentScene->AddActor(PM);
				printf("Cube Created\n");
				m_Physics->reRegisterPhysicalActors();
			}
		}
		state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_N);
		if (state == GLFW_PRESS) {
			PM = PModel::CreateSphere(m_Renderer,AddVertex3d(m_Camera->getPosition(), ScaleVertex3d(m_Camera->getDirection(), 5.0f)),  {0.0f,0.0f,1.0f}, {0.0f,1.0f,0.0f});
			if (PM) {
				PM->setRenderer(m_Renderer);
				m_CurrentScene->AddActor(PM);
				printf("sphere Created\n");
				m_Physics->reRegisterPhysicalActors();
			}
		}
		m_Physics->Update(m_Timer->getDeltaTime());
		for (_u32b i = 0; i < m_CurrentScene->getNBActors(); ++i) {
			if (m_CurrentScene->getActor(i)->getID() & DYNAMIC) {
				Dynamic* object = dynamic_cast<Dynamic*>(m_CurrentScene->getActor(i));
				object->Update(1.0f / 60.0f);
			}
		}
        m_Renderer->RenderCurrentScene();
    }
};
int RGP_CORE::Test::Init(){
	int i;
    m_CurrentScene=new GameScene ();
    if(!m_CurrentScene)
        return 0 ;
    m_Renderer=new GLRenderer();
	if (!m_Renderer->InitRenderer({ "SAMPLE",800,600,5,true,512 })) {
		return 0;
	}
	m_Physics = new PhysicsEngine();
	if (!m_Physics)
		return 0;
	if (!m_Physics->Init(m_CurrentScene))
		return 0;
    m_Camera=new PerspCamera(M_PI_2, 800.0f / 600.0f,0.1f,5000.0f);
	m_Camera->setPosition({ 0.0f,7.0f,-7.0f });
	m_Camera->setOrientation({ 0.0f, -0.5f, 0.5f }, { 0.0f, 0.5f, 0.5f });
    m_CurrentScene->setCamera(m_Camera);
	
	m_Timer = new Timer();
	if (!m_Timer)
			printf("Cannot create timer \n");

	return 1 ;
};
