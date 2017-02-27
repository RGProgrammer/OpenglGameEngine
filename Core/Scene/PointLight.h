#ifndef _TTB_POINT_LIGHT_H_
#define _TTB_POINT_LIGHT_H_

#include ".//LightSource.h"

namespace TTB {
    class PointLight: public LightSource {
    public:
        PointLight();
        PointLight(Vertex3d Pos);
        PointLight(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
    private :

    };
}

#endif
