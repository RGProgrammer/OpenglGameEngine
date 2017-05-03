#ifndef _RGP_PHYSICS_ENGINE_H_
#define _RGP_PHYSICS_ENGINE_H_
#include "..//BaseActors//Physical.h"
#include "..//Scene//GameScene.h"



namespace RGP_CORE{
	class Physical ;


	enum Shape {
		NOSHAPE             =0,
		BOX_COLLIDER		=1,
		SPHERE_COLLIDER		=2,
		CAPSULE_COLLIDER	=3,
		RAYCOLLIDER			=4/*,
		MESH_COLLIDER		=5*/
	};

	//class represent general collide description
	class Collider: public BaseActor {
		protected:
			Collider(Physical*	Owner);
			Collider(Physical*	Owner, Vertex3d Pos);
			Collider(Physical*	Owner ,Vertex3d Pos , Vertex3d Dir, Vertex3d Up);
		public:
			virtual ~Collider();
			virtual void 			Destroy();
			Shape 					getColliderShape();
			Physical* 				getOwner();

		protected:
			Shape                     	m_ColliderShape ;
			Physical*					m_Owner ;
	};
	//these are specific collider types(shapes)
	class BoxCollider: public Collider {
	public:
		BoxCollider(Physical*	Owner);
		BoxCollider(Physical*   Owner,Vertex3d Pos);
		BoxCollider(Physical*   Owner,Vertex3d Pos, Vertex3d Dir,Vertex3d Up);
		void 						getData(Vertex3d* MinVertex,Vertex3d* MaxVertex);
	private:
		Vertex3d						m_MinVertex;
		Vertex3d						m_MaxVertex;

	};
	class ShpereCollider: public Collider {
		ShpereCollider(Physical*	Owner);
		ShpereCollider(Physical*   Owner,Vertex3d Pos);
		ShpereCollider(Physical*   Owner,Vertex3d Pos, Vertex3d Dir,Vertex3d Up);
		void 						getData(_float* Radius);
	private:
		_float 							m_Radius ;

	};
	class CapsuleCollider: public Collider {
		CapsuleCollider(Physical*	Owner);
		CapsuleCollider(Physical*   Owner,Vertex3d Pos);
		CapsuleCollider(Physical*   Owner,Vertex3d Pos, Vertex3d Dir,Vertex3d Up);
		void 						getData(_float* HalfTubeLen , _float* Radius);
	private:
		_float 							m_Radius;
		_float 							m_HalfTubeLength ;
	};
	class RayCollider : public Collider {

	};


///Structure that stores the collision Data
    struct CollisionData{
		Collider* object1;
		Collider* object2;
		Vertex3d  center ;
	} ;


	//Physics simulator
	class PhysicsEngine {

	public :
		PhysicsEngine();
		PhysicsEngine(GameScene * Current);
		~PhysicsEngine();
		void 					Destroy() ;
		void 					ActivateEngine(_bool Activate=true);
		_bool 					isActive();
		GameScene*				getCurrentScene();
		void 					setCurrentScene(GameScene* Scene);
		void 					SimulatePhysics(/*Delta time*/);

	private :
		void 					FreeCollisionData();
		_bool 					CollisionCheck(Physical* obj1,Physical* obj2);

		static _bool 			CollisionBetween(BoxCollider*,BoxCollider*,Vertex3d*);
		static _bool 			CollisionBetween(BoxCollider*,ShpereCollider*,Vertex3d*);
		static _bool 			CollisionBetween(BoxCollider*,CapsuleCollider*,Vertex3d*);
		static _bool 			CollisionBetween(BoxCollider*,RayCollider*,Vertex3d*);

		static _bool 			CollisionBetween(ShpereCollider*,ShpereCollider*,Vertex3d*);
		static _bool 			CollisionBetween(ShpereCollider*,CapsuleCollider*,Vertex3d*);
		static _bool 			CollisionBetween(ShpereCollider*,RayCollider*,Vertex3d*);


		static _bool 			CollisionBetween(CapsuleCollider*,CapsuleCollider*,Vertex3d*);
		static _bool 			CollisionBetween(CapsuleCollider*,RayCollider*,Vertex3d*);

		static _bool 			CollisionBetween(RayCollider*,RayCollider*,Vertex3d*);
	private:

		GameScene*					m_CurrentScene ;
		CollisionData*				m_Data ;
		_u16b						m_nbColumns;
		_u16b						m_Size ;
		_bool						m_Active ;

	};
};

#endif
