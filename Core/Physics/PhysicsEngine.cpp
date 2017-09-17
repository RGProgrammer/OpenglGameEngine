#include  "PhysicsEngine.h"

RGP_CORE::PhysicsEngine::PhysicsEngine():m_Initilialized(false),m_SelectedScene(NULL),m_DynamicWorld(NULL), m_CollisionConfiguration(NULL),
										m_CollisionDispatcher(NULL),m_BroadPhaseInterface(NULL), m_ConstraintSolver(NULL)
{
};
RGP_CORE::PhysicsEngine::~PhysicsEngine()
{
	this->Destroy();
};
void RGP_CORE::PhysicsEngine::Destroy()
{
	
	if (m_DynamicWorld) {

		//deleting
		delete m_DynamicWorld;
		m_DynamicWorld = NULL;
	}
	if (m_ConstraintSolver) {
		delete m_ConstraintSolver;
		m_ConstraintSolver = NULL;
	}
	if (m_BroadPhaseInterface) {
		delete m_BroadPhaseInterface;
		m_BroadPhaseInterface = NULL;
	}
	if (m_pairCache) {
		delete m_pairCache;
		m_pairCache = NULL;
	}
	if (m_filterCallback) {
		delete m_filterCallback;
		m_filterCallback = NULL;
	}
	if (m_CollisionDispatcher) {
		delete m_CollisionDispatcher;
		m_CollisionDispatcher = NULL;
	}
	if (m_CollisionConfiguration) {
		delete m_CollisionConfiguration;
		m_CollisionConfiguration = NULL;
	}
	

	
	
	m_SelectedScene = NULL;
	m_Initilialized = false;
};
_bool RGP_CORE::PhysicsEngine::isInitialized() 
{
	return m_Initilialized;
};
_bool RGP_CORE::PhysicsEngine::Init(GameScene*	Scene)
{
	this->Destroy();
	//init necessary objects
	m_CollisionConfiguration= new btDefaultCollisionConfiguration();
	if (!m_CollisionConfiguration)
		return m_Initilialized;

	m_filterCallback = new MyOverlapFilterCallback2();

	m_pairCache = new btHashedOverlappingPairCache();

	m_pairCache->setOverlapFilterCallback(m_filterCallback);

	m_CollisionDispatcher=new btCollisionDispatcher(m_CollisionConfiguration);
	if (!m_CollisionDispatcher)
		return m_Initilialized;

	m_BroadPhaseInterface= new btDbvtBroadphase(m_pairCache);//btSimpleBroadphase();
	if (!m_BroadPhaseInterface)
		return m_Initilialized;
	m_ConstraintSolver=new btSequentialImpulseConstraintSolver;
	if (!m_ConstraintSolver)
		return m_Initilialized;
	
	m_DynamicWorld=new btDiscreteDynamicsWorld(m_CollisionDispatcher,m_BroadPhaseInterface,m_ConstraintSolver,m_CollisionConfiguration);
	if (!m_DynamicWorld) {
		this->Destroy();
		return m_Initilialized;
	}
	//setting scene
	m_DynamicWorld->setGravity(btVector3(0, -10, 0));
	setScene(Scene);
	return true;
};
void RGP_CORE::PhysicsEngine::Update(float DeltaTime)
{
	m_DynamicWorld->stepSimulation(DeltaTime,10);
};
void  RGP_CORE::PhysicsEngine::setScene(GameScene* Scene)
{
	m_SelectedScene=Scene;
	reRegisterPhysicalActors();
	m_DynamicWorld->applyGravity();
};
void RGP_CORE::PhysicsEngine::reRegisterPhysicalActors() 
{
	_u32b numActors=0;
	Physical* ptr=NULL ;
	if (m_SelectedScene && m_DynamicWorld) {
		// clear old values 
		m_DynamicWorld->clearForces();
		m_DynamicWorld->getNumCollisionObjects();
		for (int i = m_DynamicWorld->getNumCollisionObjects() - 1; i >= 0; --i) {
			btCollisionObject* object = m_DynamicWorld->getCollisionObjectArray()[i];
			m_DynamicWorld->removeCollisionObject(object);
		}
		//and replace them with the new ones
		numActors = m_SelectedScene->getNBActors();
		for (_u32b i = 0; i < numActors; ++i) {
			if (m_SelectedScene->getActor(i)->getID() & PHYSICAL) {
				ptr = dynamic_cast<Physical*>(m_SelectedScene->getActor(i));
				//register 
				if (ptr->getRigidBody())
					m_DynamicWorld->addRigidBody(ptr->getRigidBody());
			}
		}
	}
};