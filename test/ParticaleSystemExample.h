#ifndef _RGP_PARTICALESYSTEM_EXAMPLE_H_
#define _RGP_PARTICALESYSTEM_EXAMPLE_H_

#include "../Core/AdvancedActors/BaseParticaleSystem.h"
#include "../Core/Graphics/Model3D.h"

#define MAX_INJECTED		1000

using namespace RGP_CORE;
class Generator : public EmissionPointGenerator {
public:
	Generator(Vertex3d Pos,
				Vertex3d Dir,
				Vertex3d Up);
	void		getEmissionPoint(Vertex3d* Pos);
	void		getEmissionOrientation(Vertex3d* Dir, Vertex3d* Up);
private:
	Vertex3d		m_Pos, m_Dir, m_Up;
};
class ExampleParticale : public Particale {
public : 
	ExampleParticale();
	ExampleParticale(Vertex3d Pos);
	ExampleParticale(Vertex3d Pos,Vertex3d Dir ,Vertex3d Up);
	ExampleParticale(ExampleParticale& Source );//cloning Constrctor
	~ExampleParticale();
	void Destroy();
	_bool Init();
	void Render(Camera* Selected);
	void CastShadow();
	Particale*	CreateClone();

private :
	Model3D*		m_Model;
	_bool			m_ResourceOwner;
	
};

class ExampleEmitter : public ParticaleEmitter {
public :
	ExampleEmitter();
	ExampleEmitter(Vertex3d Pos);
	~ExampleEmitter();
	void Destroy();
	void Update(_float dt);
	_bool Init(PhysicsEngine* Engine ,Particale* sample);
private :
	_bool			AddParticale(Particale*	ptr);
	Particale**		m_Instances;
	_u32b			m_Cursor;
	_u32b			m_NumInjected;
	_float			m_time;

};

#endif