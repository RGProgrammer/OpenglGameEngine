#include "ParticaleSystemExample.h"



Generator::Generator(Vertex3d Pos,
	Vertex3d Dir,
	Vertex3d Up):m_Pos(Pos),m_Dir(Dir),m_Up(Up)
{};
void		Generator::getEmissionPoint(Vertex3d* Pos)
{
	if (Pos) {
		Pos->x = m_Pos.x;
		Pos->y = m_Pos.y;
		Pos->z = m_Pos.z;
	}
};
void		Generator::getEmissionOrientation(Vertex3d* Dir, Vertex3d* Up) 
{
		if (Dir) {
			Dir->x = m_Dir.x;
			Dir->y = m_Dir.y;
			Dir->z = m_Dir.z;
			if (Up) {
				Up->x = m_Up.x;
				Up->y = m_Up.y;
				Up->z = m_Up.z;
			}
		}
};


ExampleParticale::ExampleParticale():Particale(),m_Model(NULL),m_ResourceOwner(true)
{
	this->setShadowCast(false);
	btCollisionShape* shape = NULL;
	btTransform Transform;
	Transform.setIdentity();
	shape = new btSphereShape(2.0);
	this->AddCollider(shape, Transform);
};
ExampleParticale::ExampleParticale(Vertex3d Pos) :Particale(), m_Model(NULL), m_ResourceOwner(true)
{
	this->setShadowCast(false);
	btCollisionShape* shape = NULL;
	btTransform Transform;
	Transform.setIdentity();
	shape = new btSphereShape(2.0);
	this->AddCollider(shape, Transform);
};
ExampleParticale::ExampleParticale(Vertex3d Pos, Vertex3d Dir, Vertex3d Up) :Particale(), m_Model(NULL), m_ResourceOwner(true)
{
	this->setShadowCast(false);
	btCollisionShape* shape = NULL;
	btTransform Transform;
	Transform.setIdentity();
	shape = new btSphereShape(2.0);
	this->AddCollider(shape, Transform);
};
ExampleParticale::ExampleParticale(ExampleParticale& Source) :Particale(Source.getPosition(),Source.getDirection(),Source.getUp()),
																m_Model(Source.m_Model), m_ResourceOwner(false)
{
	this->setShadowCast(false);
	btCollisionShape* shape = NULL;
	btTransform Transform;
	Transform.setIdentity();
	shape = new btSphereShape(2.0);
	this->AddCollider(shape, Transform);
};
ExampleParticale::~ExampleParticale()
{
	this->Destroy();
};
void ExampleParticale::Destroy()
{
	if (m_ResourceOwner &&  m_Model) {
		delete m_Model;
	}
	m_Model = NULL;
	m_ResourceOwner=true ;
	Particale::Destroy();

};
_bool ExampleParticale::Init()
{
	if (!m_ResourceOwner)
		return true;

	if (m_Model) {
		delete m_Model;
		m_Model = NULL;
	}
	m_Model = new Model3D();
	if (!m_Model)
		return false;
	m_Model->setRenderer(m_GLRenderer);
	if (!m_Model->LoadModelFromFile("..//test//Samples//Sphere.obj"))
		return false;

	return true;

};
void ExampleParticale::Render(Camera* Selected)
{
	this->Update(0.0);
	m_Model->setPosition(this->getPosition());
	m_Model->setOrientation(this->getDirection(), this->getUp());
	m_Model->Render(Selected);
};
void ExampleParticale::CastShadow()
{

};
Particale*	ExampleParticale::CreateClone()
{
	ExampleParticale* Clone = NULL;
	Clone = new ExampleParticale(*this);
	if (!Clone)
		return NULL;
	return Clone;
};

///////////////////////////////////////////////////////////

ExampleEmitter::ExampleEmitter():ParticaleEmitter(),m_Instances(NULL),m_NumInjected(0),m_Cursor(0),m_time(0.0)
{};
ExampleEmitter::ExampleEmitter(Vertex3d Pos):ParticaleEmitter(), m_Instances(NULL), m_NumInjected(0), m_Cursor(0), m_time(0.0)
{};
ExampleEmitter::~ExampleEmitter()
{
	this->Destroy();
};
void ExampleEmitter::Destroy()
{
	if (m_Instances) {
		free(m_Instances);
		m_Instances = NULL;
	}
	m_NumInjected = NULL;
	m_Cursor = 0;
	m_time = 0.0;
	ParticaleEmitter::Destroy();
};
void ExampleEmitter::Update(_float dt)
{
	
	Particale* instance = NULL;
	m_time += dt;
	if (m_time >= m_EmissionRate) {
		m_time = 0.0;
		if (m_NumInjected < MAX_INJECTED) {
			instance = Emit();
			if (instance) {
				AddParticale(instance);
				instance->setPosition(this->getPosition());
				instance->setOrientation(this->getDirection(), this->getUp());
				instance->setCurrentVolecity(ScaleVertex3d(this->getUp(), 30.0));
			}
		}
		else {
			
			m_Instances[m_Cursor]->setPosition(this->getPosition());
			m_Instances[m_Cursor]->setOrientation(this->getDirection(), this->getUp());
			m_Instances[m_Cursor]->setCurrentVolecity(ScaleVertex3d(this->getUp(),30.0));
			++m_Cursor;
			if (m_Cursor >= m_NumInjected)
				m_Cursor = 0;
		}
	}
	
};
_bool ExampleEmitter::Init(PhysicsEngine* Engine, Particale* sample)
{
	if (!sample || !Engine)
		return false;
	this->setParticleSample(sample);
	Generator* generator = new Generator(this->getPosition(), this->getDirection(),this->getUp() );
	if (!generator)
		return false;
	this->setEmissionPointGenerator(generator);
	this->setPhysicsEngine(Engine);

	return true;
};
_bool ExampleEmitter::AddParticale(Particale*	ptr) 
{
	Particale** tmp = (Particale**)malloc((m_NumInjected + 1) * sizeof(Particale*));
	if (tmp) {
		if(m_Instances)
			for (_u32b i = 0; i < m_NumInjected; ++i)
				tmp[i] = m_Instances[i];
		tmp[m_NumInjected] = ptr;
		if (m_NumInjected) {
			free(m_Instances);
		}
		m_Instances = tmp;
		++m_NumInjected;
		return true;
	}
	return false;
};