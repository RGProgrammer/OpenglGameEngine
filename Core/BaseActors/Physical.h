#ifndef _TTB_PHYSICAL_TYPE_H_
#define _TTB_PHYSICAL_TYPE_H_

#include ".//Dynamic.h"
#include "..//Physics//PhysicsEngine.h"


//Physical actor type ID
#define PHYSICAL              0x00000004


namespace TTB {
	//forward Declaration
	class Collider ;
	typedef struct CollisionData CollisionData ;

	//Physical type(for physics engine)
	class Physical : public virtual Dynamic{
	protected:
		Physical();
		Physical(Vertex3d Pos);
		Physical(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
	public:
		virtual ~Physical();
		virtual void 				Destroy();
		virtual void 				React(CollisionData Data)=0;
		virtual void				Update(_float DeltaTime);
		virtual void 				ApplyTranslationForce(Vertex3d Force);
		virtual void 				ApplyRotationForce(Vertex3d Force);
		virtual _u16b				AddCollider(Collider* collider);
		virtual _u16b				getNbColliders();
		const Collider*				getColliderByIndex(_u16b Index);

	protected:
		_u16b							m_nbColliders;
		Collider**						m_Colliders ;
		Vertex3d						m_AppliedTranslationForce ;
		Vertex3d						m_AppliedRotationForce;
		Vertex3d						m_Velocity ;
		_float  						m_Mass ;
	};
};
#endif
