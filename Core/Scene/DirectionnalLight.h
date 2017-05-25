#ifndef _RGP_DIRECTIONNAL_LIGHT_H_
#define _RGP_DIRECTIONNAL_LIGHT_H_

#include ".//LightSource.h"

namespace RGP_CORE {

    class DirectionnalLight : public LightSource {
    public:
        DirectionnalLight();
        DirectionnalLight(Vertex3d Pos);
        DirectionnalLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
	protected:
		void UpdateLightProjectionMtx();
    };
};

#endif
