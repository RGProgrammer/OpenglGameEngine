#ifndef _TTB_DIRECTIONNAL_LIGHT_H_
#define _TTB_DIRECTIONNAL_LIGHT_H_

#include ".//LightSource.h"

namespace TTB {

    class DirectionnalLight : public LightSource {
    public:
        DirectionnalLight();
        DirectionnalLight(Vertex3d Pos);
        DirectionnalLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
    };
};

#endif
