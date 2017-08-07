#ifndef _RGP_PHYSICAL_TYPE_H_
#define _RGP_PHYSICAL_TYPE_H_

#include ".//Dynamic.h"
#include "..//Physics//PhysicsEngine.h"


//Physical actor type ID
#define PHYSICAL              0x00000004


namespace RGP_CORE {
	//Physical type(for physics engine)
	class Physical : public virtual Dynamic{
	protected:
		Physical();
		Physical(Vertex3d Pos);
		Physical(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
	public:
		virtual ~Physical();
		virtual void 				Destroy();
		//virtual void 				React()=0;
		virtual void				Update(_float DeltaTime);
		virtual void 				ApplyTranslationForce(Vertex3d Force);
		virtual void 				ApplyRotationForce(Vertex3d Force);
		virtual _u16b				AddCollider(btCollisionShape* collider, const btTransform& transfom);
		virtual _u16b				getNbColliders();
		btCollisionShape*			getColliderByIndex(_u16b Index);
		btRigidBody*				getRigidBody();
		//btSoftBody*					getSoftBody();
		void						setPosition(Vertex3d Pos);
		_bool						setOrientation(Vertex3d Dir, Vertex3d Up);

	protected:
		_u16b							m_nbCollisionShapes;
		btCollisionShape**				m_CollisionShapes;
		btCollisionShape*				m_Collider;
		btRigidBody*					m_Rigidbody;
		
		//btSoftBody*						m_SoftBody;
		Vertex3d						m_Velocity ;
		_float  						m_Mass ;
	};
};
#endif
