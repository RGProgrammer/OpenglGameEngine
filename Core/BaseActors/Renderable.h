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
		Renderable(const _s8b* name = "Rendrable");
		Renderable(Vertex3d Pos, const _s8b* name = "Rendrable");
		Renderable(Vertex3d Pos,Vertex3d Dir,Vertex3d Up, const _s8b* name = "Rendrable");
	public:
		virtual ~Renderable();
		virtual void Destroy();
		virtual void Render(Camera* Selected)=0;
		virtual void CastShadow() = 0;
		void	setShadowCast(_bool Value);
		_bool   doesShadowCast();
		void	setVisible(_bool visible = true);
		_bool	isVisible();
		void	setEffectedByLight(_bool value = true);
		_bool	isEffectedByLight();
		void	setRenderer(GLRenderer* renderer);
		
		//virtual _bool isEnvMap() = 0;
		
    protected:
        GLRenderer*             m_GLRenderer ;
		_bool					m_DoesCastShadow;
		_bool					m_EffectByLight;
    private:
        _bool                   m_Visible ;
	};

};

#endif
