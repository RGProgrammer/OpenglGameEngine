#ifndef TTB_WINDOW_HANDLER_H_
#define TTB_WINDOW_HANDLER_H_
#include "..//Common//BasePrimitiveTypes.h"
#include "glHeaders.h"


namespace RGP_CORE{

	class Window {
	public:
	    static Window* Create(char* title,_u32b width, _u32b height, bool Fullscreen=false);
		static Window* Create(Window& Main);
		Window();
		~Window();
		void Destroy();
		_u32b InitWindow(char* title,_u32b width, _u32b height, bool Fullscreen=false);
		_u32b InitWindow(Window& Main);
		void setFullScreen(bool Value);
		bool isFullScreen();
		void Reszie(_u32b Width,_u32b Height);
		_u32b getHeight();
		_u32b getWidth();
		GLFWwindow* getglfwWindow();
	private:
		GLFWwindow*					m_ptrWindow ;
		_u32b 						m_Height;
		_u32b 						m_Width;
		bool						m_FullScreen;
	};
};
#endif
