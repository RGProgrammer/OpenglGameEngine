#ifndef _TEST_GLOWING_PLANE_H_
#define _TEST_GLOWING_PLANE_H_

#include "../Core/Graphics/Model3D.h"
using namespace RGP_CORE;

class EmissiveObject : public Model3D {
public :
	static EmissiveObject* Create(GLRenderer* renderer);
private :
	EmissiveObject();
public:
	~EmissiveObject();
	void Destroy();
	_bool init();

};


#endif
