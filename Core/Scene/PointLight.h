#ifndef _RGP_POINT_LIGHT_H_
#define _RGP_POINT_LIGHT_H_

#include ".//LightSource.h"

namespace RGP_CORE {
    class PointLight: public virtual LightSource {
    public:
		static BaseActor*	Create(void ** args);
        PointLight();
        PointLight(Vertex3d Pos);
        PointLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
    protected :
		void	UpdateLightViewMtx();
		void	UpdateLightProjectionMtx(Camera* Selected=NULL);

    };
}

#endif
