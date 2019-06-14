#ifndef _RGP_PHYSICAL_MODEL_H_
#define _RGP_PHYSICAL_MODEL_H_
#include "..//BaseActors//Physical.h"
#include "..//Graphics//Model3D.h"


namespace RGP_CORE {
	class PhysicalModel : public virtual Physical, public virtual Model3D 
	{
	public:
		PhysicalModel(_float mass=0.0 );
		PhysicalModel(Vertex3d Pos, _float mass = 0.0);
		PhysicalModel(Vertex3d Pos, Vertex3d Dir, Vertex3d Up, _float mass = 0.0);
		~PhysicalModel();
		virtual void Destroy();
	};

}

#endif