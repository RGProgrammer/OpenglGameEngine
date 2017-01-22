#ifndef _TTB_PARTICLE_SYSTEM_H_
#define _TTB_PARTICLE_SYSTEM_H_
#include "..//BaseActorTypes//Physical.h"
namespace TTB {
	class Particle : virtual public Physical{
	public:
		Particle();
		Particle(Vertex3d Pos);
		Particle(Vertex3d Pos,Vertex3d Dir,Vertex3d Up);
		virtual ~Particle();
		virtual void Destory();
		virtual void Update(_float DeltaTime);
		virtual void ApplyForce(Vertex3d Force);
	}

	class ParticleEmitter : virtual public Renderable,virtual public Physical {
		public:
			ParticleEmitter();
			ParticleEmitter(Vertex3d Pos);
			virtual ~ParticleEmitter();
			virtual void 			Destory();
			virtual void			Render(_float* Matrix);
			virtual void			Update(_float DeltaTime);
			virtual void 			React(CollisionData collision);
			virtual void			Emit(Vertex3d Pos);
	}

#endif