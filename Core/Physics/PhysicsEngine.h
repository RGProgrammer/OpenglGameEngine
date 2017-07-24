#ifndef _RGP_PHYSICS_ENGINE_H_
#define _RGP_PHYSICS_ENGINE_H_
#include "..//Scene//GameScene.h"
#include "..//..//tools//bullet//btBulletDynamicsCommon.h"
#include "..//..//tools//bullet//btBulletCollisionCommon.h"
#include "..//..//tools//bullet//LinearMath//btTransform.h"
#include "..//..//tools//bullet//LinearMath//btTransformUtil.h"
#include "..//..//tools//bullet//BulletSoftBody//btSoftBody.h"
#include "..//BaseActors//Physical.h"


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
		btDefaultCollisionConfiguration*		m_CollisionConfiguration;
		btCollisionDispatcher*					m_CollisionDispatcher;
		btBroadphaseInterface*					m_BroadPhaseInterface;
		btSequentialImpulseConstraintSolver*	m_ConstraintSolver;
	};
};

#endif
