#ifndef _RGP_RENDERABLE_TYPE_H_
#define _RGP_RENDERABLE_TYPE_H_

#include ".//BaseActor.h"
#include "..//Graphics//glHeaders.h"
#include "..//Graphics//GLRenderer.h"
#include "..//Scene//Camera.h"

//Renderable actor type ID
#define RENDERABLE              0x00000001



namespace RGP_CORE{

class GLRenderer ;
///Renderable Type
class Renderable: public virtual BaseActor {
	protected:
		Renderable();
		Renderable(Vertex3d Pos);
		Renderable(Vertex3d Pos,Vertex3d Dir,Vertex3d Up);
	public:
		virtual ~Renderable();
		virtual void Destroy();
		virtual void Render(Camera* Selected)=0;
		virtual void CastShadow() = 0;
		void	setRenderer(GLRenderer* renderer);
		void	setVisible(_bool visible=true);
		_bool	isVisible();
		void	setShadowCast(_bool Value);
		_bool   doesShadowCast();
    protected:
        GLRenderer*             m_GLRenderer ;
		_bool					m_DoesCastShadow;
    private:
        _bool                   m_Visible ;
	};

};

#endif
