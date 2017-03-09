#include "Test.h"

TTB::Test::Test():m_Renderer(NULL),m_CurrentScene(NULL),m_Camera(NULL){
};
TTB::Test::~Test(){
    this->Destroy() ;
};
void TTB::Test::Destroy(){
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
};
void TTB::Test::Start(){

    TTB::Model3D* testmodel1=new Model3D();
    testmodel1->setRenderer(m_Renderer);
    if(!testmodel1->LoadModelFromFile(".//test//Sky//skybox.obj"))
        printf("error loading test Model\n");
    m_CurrentScene->AddActor(testmodel1);
    TTB::Model3D* testmodel2=new Model3D();
    testmodel2->setRenderer(m_Renderer);
    if(!testmodel2->LoadModelFromFile(".//test//test1.obj"))
        printf("error loading test Model\n");
    m_CurrentScene->AddActor(testmodel2);

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
            m_Camera->RotateViaSide(-0.005);
        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_DOWN);
        if (state == GLFW_PRESS){
            m_Camera->RotateViaSide(0.005);
        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_LEFT);
        if (state == GLFW_PRESS){
            m_Camera->RotateViaUp(0.005);
        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_RIGHT);
        if (state == GLFW_PRESS){
            m_Camera->RotateViaUp(-0.005);
        }
        m_Renderer->RenderCurrentScene();
    }
};
int TTB::Test::Init(){

    m_CurrentScene=new GameScene ();
    if(!m_CurrentScene)
        return 0 ;
    m_Renderer=new GLRenderer();
    if(!m_Renderer->InitRenderer({"SAMPLE",800,600,5}))
        return 0 ;
    m_Camera=new PerspCamera(45.0f,800.0f/600.0f,1.0f,5000.0f);
    m_Camera->Translate({0.0f,0.0f,50.0f});
    m_CurrentScene->setCamera(m_Camera);
    m_Renderer->setScene(m_CurrentScene);
    ///add some models to the scene for test
    LightSource* light=NULL ;

    /*light=new DirectionnalLight();
    light->setOrientation({0.0f,0.0f,1.0f},{0.0f,1.0f,0.0f});
    m_CurrentScene->AddLight(light);*/


    light= new PointLight({0.0f,-20.0f,0.0f});
    m_CurrentScene->AddLight(light);

    light= new PointLight({20.0f,0.0f,0.0f});
    m_CurrentScene->AddLight(light);

    light= new PointLight({-20.0f,0.0f,0.0f});
    m_CurrentScene->AddLight(light);

    light= new PointLight({0.0f,0.0f,20.0f});
    m_CurrentScene->AddLight(light);



    ///ecerything is good
    return 1 ;


};
