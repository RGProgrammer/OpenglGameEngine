#include "Test.h"

TTB::Test::Test():m_Renderer(NULL),m_CurrentScene(NULL),m_Camera(NULL){
};
TTB::Test::~Test(){
    this->Destroy() ;
};
void TTB::Test::Destroy(){
    printf("Destroy and release resources \n");
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
    printf("Starting game loop\n");
    while(true){
        int state;
        glfwPollEvents();
        if(glfwWindowShouldClose(m_Renderer->getTarget()->getglfwWindow()))
            break ;
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_W);
        if (state == GLFW_PRESS){

        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_S);
        if (state == GLFW_PRESS){

        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_D);
        if (state == GLFW_PRESS){

        }
        state = glfwGetKey(m_Renderer->getTarget()->getglfwWindow(), GLFW_KEY_A);
        if (state == GLFW_PRESS){

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
    m_Camera=new PerspCamera(45.0f,800.0f/600.0f,0.0f,500.0f);
    m_Camera->Translate({0.0f,0.0f,100.0f});
    m_CurrentScene->setCamera(m_Camera);
    m_Renderer->setScene(m_CurrentScene);
    ///add some models to the scene for test
    Model3D* testmodel=NULL ;

    testmodel=new Model3D();
    testmodel->setRenderer(m_Renderer);
    if(!testmodel->LoadModelFromFile(".//test//Sky//skybox.obj"))
        printf("error loading test Model\n");
    m_CurrentScene->AddActor(testmodel);
    testmodel=new Model3D();
    testmodel->setRenderer(m_Renderer);
    if(!testmodel->LoadModelFromFile(".//test//test1.obj"))
        printf("error loading test Model\n");
    m_CurrentScene->AddActor(testmodel);



    ///ecerything is good
    return 1 ;


};
