#ifndef TTB_ACTOR_BASE_TYPES_H_
#define TTB_ACTOR_BASE_TYPES_H_



#include "..//Common//BasePrimitiveTypes.h"
#include "..//Common//Math.h"
#include <stdlib.h>
#include <stdio.h>
///////////////////////////////////////////////////////////////////////
// to Identity each type actor type we some sort of Identifiers(IDs) //
///////////////////////////////////////////////////////////////////////
#define  UNKNOWN		0x00000000


namespace TTB{
//Actors base types
	class BaseActor {
	protected :
		BaseActor(Vertex3d Pos,Vertex3d Dir,Vertex3d Up);
		BaseActor(Vertex3d Pos);
		BaseActor();
	public:
		virtual ~BaseActor       ();
		virtual void Destroy ();
		_u32b getID          ();
		void setPosition     (Vertex3d Pos);
		Vertex3d getPosition ();
		bool setOrientation  (Vertex3d Dir, Vertex3d Up);
		Vertex3d getDirection();
		Vertex3d getUp       ();
		Vertex3d getLeft     ();
		void RotateViaDirection (_float Angle);
		void RotateViaUp        (_float Angle);
		void RotateViaLeft      (_float Angle);
		void Translate          (Vertex3d ver);
		void 					ScaleUniform(_float value) ;
		void 					Scale(Vertex3d scale);
		Vertex3d				getScale();

		_float* getTransMtx();

	protected:
		void UpdateTransMtx();

		_u32b						m_ID ;
		Vertex3d 					m_Position;
		Vertex3d					m_Direction;
		Vertex3d					m_Up;
		Vertex3d					m_Scale ;
		_float*						m_TransMtx;/// 4*4 matrix

	};

};
#endif