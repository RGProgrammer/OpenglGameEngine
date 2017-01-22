#ifndef TTB_WINDOW_HANDLER_H_
#define TTB_WINDOW_HANDLER_H_
#include "..//Common//BasePrimitiveTypes.h"
#include "glHeaders.h"


namespace TTB{

	class Window {
	public:
	    static Window* CreateWindow(char* title,_u16b width, _u16b height, bool Fullscreen=false);
		Window();
		~Window();
		void Destroy();
		_u16b InitWindow(char* title,_u16b width, _u16b height, bool Fullscreen=false);
		void setFullScreen(bool Value);
		bool isFullScreen();
		void Reszie(_u16b Width,_u16b Height);
		_u16b getHeight();
		_u16b getWidth();
		GLFWwindow* getglfwWindow();
	private:
		GLFWwindow*					m_ptrWindow ;
		_u16b 						m_Height;
		_u16b 						m_Width;
		bool						m_FullScreen;
	};
};
#endif
