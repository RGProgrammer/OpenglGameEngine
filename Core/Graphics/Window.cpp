#include "Window.h"

TTB::Window* TTB::Window::CreateWindow(char* title,_u16b width, _u16b height, bool Fullscreen){
    Window *Target=NULL;
    Target=new Window();
    if(!Target)
        return NULL ;
    if(!Target->InitWindow(title,width,height,Fullscreen)){
        Target->Destroy();
        delete Target ;
        return NULL;
    }
    return Target ;
};

TTB::Window::Window(): m_ptrWindow(0),m_Height(0),m_Width(0),m_FullScreen(false){
};
TTB::Window::~Window(){
	this->Destroy();
};
void TTB::Window::Destroy(){
    if(m_ptrWindow){
        glfwDestroyWindow(m_ptrWindow);
        m_ptrWindow=0;
    }
    this->m_Height=this->m_Width=0;
    m_FullScreen=false ;
};
_u16b TTB::Window::InitWindow(char* Title,_u16b Width, _u16b Height, bool Fullscreen){
	m_ptrWindow=glfwCreateWindow(Width,Height,Title,NULL,NULL);
	if(m_ptrWindow==NULL)
        return 0 ;
    m_Height=Height;
    m_Width=Width ;
    m_FullScreen=Fullscreen ;
    return 1;
};
void TTB::Window::setFullScreen(bool Value){

};

bool TTB::Window::isFullScreen(){ return m_FullScreen ;};

void TTB::Window::Reszie(_u16b Width,_u16b Height){

};
_u16b TTB::Window::getHeight(){ return m_Height ;};
_u16b TTB::Window::getWidth(){ return m_Width ;};
GLFWwindow* TTB::Window::getglfwWindow(){ return m_ptrWindow; };
