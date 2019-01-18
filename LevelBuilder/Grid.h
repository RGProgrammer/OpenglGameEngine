#ifndef _RGP_GRID_RENDREBALE_H_
#define _RGP_GRID_RENDREBALE_H_
#include "..//..//Core//BaseActors//Renderable.h"

namespace RGP_LEVELBUILDER {
	class Grid : public RGP_CORE::Renderable 
	{
	public:
		Grid();
		~Grid();
		void Destroy();
		void Render(RGP_CORE::Camera* Selected);

	private:
		GLuint						m_ShaderProgram;
		
	};
}


#endif
