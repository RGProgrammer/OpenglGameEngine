#ifndef _RGP_PHYSICS_ENGINE_H_
#define _RGP_PHYSICS_ENGINE_H_
#include "..//Scene//GameScene.h"
#include "..//..//tools//bullet3//btBulletCollisionCommon.h"
#include "..//..//tools//bullet3//btBulletDynamicsCommon.h"
#include "..//BaseActors//Physical.h"


//hiding the Bullet physics API behind a simpler API 
//this interface is not complete yet, it's mostly usable.



enum MyFilterModes
{
	FILTER_GROUPAMASKB_AND_GROUPBMASKA2 = 0,
	FILTER_GROUPAMASKB_OR_GROUPBMASKA2
};

struct MyOverlapFilterCallback2 : public btOverlapFilterCallback
{
	int m_filterMode;

	MyOverlapFilterCallback2()
		:m_filterMode(FILTER_GROUPAMASKB_AND_GROUPBMASKA2)
	{
	}

	virtual ~MyOverlapFilterCallback2()
	{}
	// return true when pairs need collision
	virtual bool	needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
	{
		if (m_filterMode == FILTER_GROUPAMASKB_AND_GROUPBMASKA2)
		{
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
			return collides;
		}

		if (m_filterMode == FILTER_GROUPAMASKB_OR_GROUPBMASKA2)
		{
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides || (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
			return collides;
		}
		return false;
	}
};


namespace RGP_CORE{
	class Physical ;
	//Physics simulator
	class PhysicsEngine {
	public:
		PhysicsEngine();
		~PhysicsEngine();
		virtual void Destroy();
		_bool Init(GameScene*	Scene);
		void Update(float DeltaTime);
		void setScene(GameScene* Scene);
		void reRegisterPhysicalActors();
	private :
		_bool isInitialized();

	protected :
		_bool									m_Initilialized; 
		GameScene*								m_SelectedScene;
		btDiscreteDynamicsWorld*				m_DynamicWorld;
		MyOverlapFilterCallback2*				m_filterCallback;
		btOverlappingPairCache*					m_pairCache;
		btDefaultCollisionConfiguration*		m_CollisionConfiguration;
		btCollisionDispatcher*					m_CollisionDispatcher;
		btBroadphaseInterface*					m_BroadPhaseInterface;
		btSequentialImpulseConstraintSolver*	m_ConstraintSolver;
	};
};

#endif
