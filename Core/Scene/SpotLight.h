#ifndef _TTB_SPOT_LIGHT_H_
#define _TTB_SPOT_LIGHT_H_

#include ".//LightSource.h"

namespace TTB {
    class SpotLight : virtual public LightSource {
    public:
        SpotLight();
        SpotLight(Vertex3d Pos);
        SpotLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
    };
}

#endif
