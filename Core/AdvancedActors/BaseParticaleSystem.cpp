#include "BaseParticaleSystem.h"



RGP_CORE::Particale::Particale():Renderable(), Physical()
{};
RGP_CORE::Particale::Particale(Vertex3d Pos) :Renderable(), Physical(Pos)
{};
RGP_CORE::Particale::Particale(Vertex3d Pos, Vertex3d Dir, Vertex3d Up) :Renderable(),
																		Physical(Pos,Dir,Up)
{};
RGP_CORE::Particale:: ~Particale()
{
	this->Destroy();
};
void RGP_CORE::Particale::Destroy()
{
	Renderable::Destroy();
	Physical::Destroy();
};


RGP_CORE::ParticaleEmitter::ParticaleEmitter() : Dynamic(), m_Scene(NULL), m_ptrPhysicsEngine(NULL), m_EPGenerator(NULL), m_Sample(NULL ),
												m_EmissionRate(0.2), m_CloneStartSpeed(1.0)
{};
RGP_CORE::ParticaleEmitter::ParticaleEmitter(Vertex3d Pos):Dynamic(Pos),m_Scene(NULL), m_ptrPhysicsEngine(NULL), m_EPGenerator(NULL), m_Sample(NULL),
																		m_EmissionRate(0.2), m_CloneStartSpeed(1.0)
{};
RGP_CORE::ParticaleEmitter::~ParticaleEmitter()
{
	Destroy();
};
void		RGP_CORE::ParticaleEmitter::Destroy()
{
	m_Scene = NULL;;
	if (m_EPGenerator) {
		delete m_EPGenerator;
		m_EPGenerator = NULL;
	}
	if (m_Sample) {
		delete m_Sample;
		m_Sample = NULL;
	}
	m_ptrPhysicsEngine=NULL;
	m_EmissionRate = 0.0;
	m_CloneStartSpeed=0.0 ;
};
/*void		RGP_CORE::ParticaleEmitter::Update(_float dt)
{
	
};*/
void		RGP_CORE::ParticaleEmitter::setScene(GameScene*		SelectedScene)
{
	m_Scene = SelectedScene;
};
void		RGP_CORE::ParticaleEmitter::setPhysicsEngine(PhysicsEngine*		engine)
{
	m_ptrPhysicsEngine = engine;
};
void		RGP_CORE::ParticaleEmitter::setParticleSample(Particale*	Sample)
{
	m_Sample = Sample;
};
RGP_CORE::Particale*			RGP_CORE::ParticaleEmitter::getParticleSample()
{
	return m_Sample;
};
RGP_CORE::Particale*			RGP_CORE::ParticaleEmitter::Emit()
{
	Particale* Clone = NULL;
	Vertex3d Pos, Dir, Up;
	if (!m_Sample || !m_Scene || !m_EPGenerator || !m_ptrPhysicsEngine)
		return NULL;
	if (!(Clone = m_Sample->CreateClone()))
		return NULL;
	
	m_EPGenerator->getEmissionPoint(&Pos);
	m_EPGenerator->getEmissionOrientation(&Dir,&Up);
	Clone->setPosition(Pos);
	Clone->setOrientation(Dir,Up);
	Clone->setCurrentVolecity(ScaleVertex3d(this->getDirection(), m_CloneStartSpeed));

	if (!(m_Scene->AddActor(Clone))) {
		delete Clone;
		return NULL;
	}
	m_ptrPhysicsEngine->reRegisterPhysicalActors();
	return Clone;
};
void						RGP_CORE::ParticaleEmitter::setEmissionPointGenerator(EmissionPointGenerator*	Generator)
{
	m_EPGenerator = Generator;
};
RGP_CORE::EmissionPointGenerator*		RGP_CORE::ParticaleEmitter::getEmissionPointGenerator()
{
	return m_EPGenerator;
};
void						RGP_CORE::ParticaleEmitter::setEmissionRate(_float Rate)
{
	if (Rate >= 0.0)
		m_EmissionRate = Rate;
};
_float						RGP_CORE::ParticaleEmitter::getEmissionRate()
{
	return m_EmissionRate;	
};
void						RGP_CORE::ParticaleEmitter::setParticaleStartSpeed(_float speed)
{
	if (speed >= 0.0)
		m_CloneStartSpeed = speed;
};
_float						RGP_CORE::ParticaleEmitter::getParticaleStartSpeed()
{
	return m_CloneStartSpeed;
};