#include "MyWindow.h"

RGP_CORE::MyWindow* RGP_CORE::MyWindow::Create(char* title,_u32b width, _u32b height, bool Fullscreen){
    MyWindow *Target=NULL;
    Target=new MyWindow();
    if(!Target)
        return NULL ;
    if(!Target->InitWindow(title,width,height,Fullscreen)){
        Target->Destroy();
        delete Target ;
        return NULL;
    }
    return Target ;
};

RGP_CORE::MyWindow* RGP_CORE::MyWindow::Create(MyWindow& Main)
{
	MyWindow *Target = NULL;
	Target = new MyWindow();
	if (!Target)
		return NULL;
	if (!Target->InitWindow(Main)) {
		Target->Destroy();
		delete Target;
		return NULL;
	}
	return Target;
};

RGP_CORE::MyWindow::MyWindow(): m_ptrWindow(0),m_Height(0),m_Width(0),m_FullScreen(false){
};
RGP_CORE::MyWindow::~MyWindow(){
	this->Destroy();
};
void RGP_CORE::MyWindow::Destroy(){
    if(m_ptrWindow){
        glfwDestroyWindow(m_ptrWindow);
        m_ptrWindow=0;
    }
    this->m_Height=this->m_Width=0;
    m_FullScreen=false ;
};
_u32b RGP_CORE::MyWindow::InitWindow(char* Title,_u32b Width, _u32b Height, bool Fullscreen){
	m_ptrWindow=glfwCreateWindow(Width,Height,Title,NULL,NULL);
	if(m_ptrWindow==NULL)
        return 0 ;
    m_Height=Height;
    m_Width=Width ;
    m_FullScreen=Fullscreen ;
    return 1;
};
_u32b RGP_CORE::MyWindow::InitWindow(MyWindow& Main)
{
	m_ptrWindow = glfwCreateWindow(Main.getWidth(), Main.getHeight(),"", NULL, Main.getglfwWindow());
	if (m_ptrWindow == NULL)
		return 0;
	m_Height = Main.getHeight();
	m_Width = Main.getWidth();
	m_FullScreen = Main.isFullScreen();
	return 1;
};
void RGP_CORE::MyWindow::setFullScreen(bool Value){

};

bool RGP_CORE::MyWindow::isFullScreen(){ return m_FullScreen ;};

void RGP_CORE::MyWindow::Reszie(_u32b Width,_u32b Height){

};
_u32b RGP_CORE::MyWindow::getHeight(){ 
	glfwGetWindowSize(m_ptrWindow, (int*)&m_Width, (int*)&m_Height);
	return m_Height ;};
_u32b RGP_CORE::MyWindow::getWidth(){
	glfwGetWindowSize(m_ptrWindow, (int*)&m_Width, (int*)&m_Height);
	return m_Width ;};
GLFWwindow* RGP_CORE::MyWindow::getglfwWindow(){ return m_ptrWindow; };
