#ifndef _RGP_SPOT_LIGHT_H_
#define _RGP_SPOT_LIGHT_H_

#include ".//LightSource.h"

namespace RGP_CORE {
    class SpotLight : virtual public LightSource {
    public:
        SpotLight();
        SpotLight(Vertex3d Pos);
        SpotLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
	protected:
		void	UpdateLightViewMtx();
		void	UpdateLightProjectionMtx(Camera* Selected);
    };
}

#endif
