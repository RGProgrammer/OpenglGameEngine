#ifndef _RGP_CORE_INPUT_HANDLER_H_
#define _RGP_CORE_INPUT_HANDLER_H_

#include "..//Graphics//MyWindow.h"

namespace RGP_CORE {
	class Input {
	public :
		static Input* CreateInputHandle(Window* EventsSource);
	private : 
		Input();
		~Input();
		_bool		Init(Window* EventSource);
		void		Destroy();

	//private :

	};
}


#endif 
