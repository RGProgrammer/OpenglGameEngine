#ifndef _RGP_LIGHTSOURCE_TYPE_H_
#define _RGP_LIGHTSOURCE_TYPE_H_

#include "..//BaseActors//BaseActor.h"
#include "Camera.h"

#define LIGHTSOURCE         0x00000008

namespace RGP_CORE {

	class LightSource: virtual public BaseActor {
    protected:
        LightSource();
        LightSource(Vertex3d Pos);
        LightSource(Vertex3d Pos,Vertex3d Dir,Vertex3d Up);
    public:
        virtual ~LightSource();
		virtual void Destroy();
        void setLightDiffuseColor(_float red, _float green, _float blue);
        void setLightDiffuseColor(Vertex3d color);
        void setLightSpecularColor(_float red, _float green, _float blue);
        void setLightSpecularColor(Vertex3d color);
        void setLightAmbientColor(_float red, _float green, _float blue);
        void setLightAmbientColor(Vertex3d color);
        void setLightDistance(_float Distance);
        void setLightStrength(_float Strength);

        const _float* getLightDiffuseColor();
        const _float* getLightSpecularColor();
        const _float* getLightAmbientColor();
        _float   getLightDistance();
        _float   getLightAttinuation();
        _float   getLightCutoffAngle();
        _float   getLightShiness();

		_float*	getLightViewMtx();
		_float*	getLightProjectionMtx(Camera*	selected);
    protected:
		virtual void	UpdateLightViewMtx() = 0;
		virtual void	UpdateLightProjectionMtx(Camera*	Selected)= 0;
        _float				m_DiffuseColor[3] ;
        _float				m_SpecularColor[3] ;
        _float				m_AmbientColor[3] ;
        _float				m_Distance ;
        _float				m_CutoffAngle ;
        _float				m_Shiness ;     ///light strength
		_float*				m_LightViewMtx;
		_float*				m_LightProjectionMtx; 


	};
};
#endif
