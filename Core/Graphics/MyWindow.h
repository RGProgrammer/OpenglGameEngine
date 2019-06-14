#ifndef TTB_WINDOW_HANDLER_H_
#define TTB_WINDOW_HANDLER_H_
#include "..//Common//BasePrimitiveTypes.h"
#include "glHeaders.h"


namespace RGP_CORE{

	class MyWindow {
	public:
	    static MyWindow* Create(char* title,_u32b width, _u32b height, bool Fullscreen=false);
		static MyWindow* Create(MyWindow& Main);
		MyWindow();
		~MyWindow();
		void Destroy();
		_u32b InitWindow(char* title,_u32b width, _u32b height, bool Fullscreen=false);
		_u32b InitWindow(MyWindow& Main);
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
