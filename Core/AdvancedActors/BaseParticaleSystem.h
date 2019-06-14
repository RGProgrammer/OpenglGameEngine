#ifndef _RGP_BASEPARTICLESYSTEM_H_
#define _RGP_BASEPARTICLESYSTEM_H_
#include "..//BaseActors//Renderable.h"
#include "..//BaseActors//Dynamic.h"
#include "..//BaseActors//Physical.h"


namespace RGP_CORE {


	class EmissionPointGenerator {
	public:
		virtual void		getEmissionPoint(Vertex3d* Pos) = 0;
		virtual void		getEmissionOrientation(Vertex3d* Dir, Vertex3d* Up) = 0;
	};
	
	class Particale : public virtual Renderable, public virtual Physical {
	public:
		Particale();
		Particale(Vertex3d Pos);
		Particale(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
		virtual ~Particale();
		virtual void					Destroy();
		virtual void					Render(Camera* Selected)=0;
		virtual Particale*				CreateClone() = 0;
		_bool							isEnvMap() { return false; };
	};

	
	class ParticaleEmitter : public virtual Dynamic {
	protected:
		ParticaleEmitter();
		ParticaleEmitter(Vertex3d Pos);
	public:
		virtual ~ParticaleEmitter();
		virtual void				Destroy();
		virtual void				Update(_float dt)= 0;
		void						setScene(GameScene*		SelectedScene);
		void						setPhysicsEngine(PhysicsEngine*	engine);
		void						setParticleSample(Particale*	Sample);
		Particale*					getParticleSample();
		void						setEmissionPointGenerator(EmissionPointGenerator*	Generator);
		EmissionPointGenerator*		getEmissionPointGenerator();
		void						setEmissionRate(_float Rate);
		_float						getEmissionRate();
		void						setParticaleStartSpeed(_float speed);
		_float						getParticaleStartSpeed();


	protected :
		Particale*					Emit(); //a clone of sample will be created ijected directly in the scene
									
	protected :
		GameScene*							m_Scene;
		PhysicsEngine*						m_ptrPhysicsEngine;
		EmissionPointGenerator*				m_EPGenerator;
		Particale*							m_Sample;
		_float								m_EmissionRate;//emission speed 
		_float								m_CloneStartSpeed;
	};

}

#endif