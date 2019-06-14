#ifndef _RGP_GRID_RENDREBALE_H_
#define _RGP_GRID_RENDREBALE_H_
#include "../Core/BaseActors/Renderable.h"
using namespace RGP_CORE;
namespace RGP_LEVELBUILDER {
	class Grid : public RGP_CORE::Renderable 
	{
	public :
		static Grid* CreateGrid(GLRenderer* renderer);
	private:
		//static Grid* singleton;
		Grid();
		_bool Init();
	public :
		~Grid();
		void Destroy();
		virtual void Render(RGP_CORE::Camera* Selected) override;
		virtual void CastShadow() override;

	private:
		_u32b						m_ShaderProgram;
		_u32b						m_VAO;
		_u32b						m_VertexBuffer;
		
	};
}


#endif
