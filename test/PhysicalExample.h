#ifndef _RGP_PHYSICAL_OBJECTS_EXAMPLE_H_
#define _RGP_PHYSICAL_OBJECTS_EXAMPLE_H_
#include "..//Core//Graphics//Model3D.h"
#include "..//Core//BaseActors/Physical.h"

using namespace RGP_CORE;

class PModel : virtual public Model3D, virtual public Physical {
public :
	static PModel*	CreateCube(GLRenderer* renderer, Vertex3d Pos, Vertex3d Dir, Vertex3d Up );
	static PModel*	CreateSphere(GLRenderer* renderer, Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
	//static PModel*	CreateCapsule();
private :
	PModel(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);;
public :
	~PModel();
	void Destroy();


};

#endif