#ifndef _RGP_ACTOR_BASE_TYPE_H_
#define _RGP_ACTOR_BASE_TYPE_H_



#include "..//Common//BasePrimitiveTypes.h"
#include "..//Common//Math.h"
#include "..//ClassesDB//ClassesDB.h"
#include <stdlib.h>
#include <stdio.h>
///////////////////////////////////////////////////////////////////////
// to Identity each type actor type we some sort of Identifiers(IDs) //
///////////////////////////////////////////////////////////////////////
#define  UNKNOWN		0x00000000


namespace RGP_CORE {
//Actors base types
	class BaseActor {
	protected :
		BaseActor(Vertex3d Pos,Vertex3d Dir,Vertex3d Up, const _s8b* name="Actor");
		BaseActor(Vertex3d Pos, const _s8b* name="Actor");
		BaseActor(const _s8b* name="Actor");
	public:
		virtual ~BaseActor   ();
		virtual void Destroy ();
		_s8b*		getName() {return m_Name;}
		_bool		setName(const _s8b* name);
		_u32b		getID          ();
		Vertex3d	getPosition();
		Vertex3d	getDirection();
		Vertex3d	getUp       ();
		Vertex3d	getSide     ();
		Vertex3d	getScale();
		virtual void setPosition(Vertex3d Pos);
		virtual bool setOrientation(Vertex3d Dir, Vertex3d Up);
		virtual void RotateViaDirection (_float Angle);
		virtual void RotateViaUp        (_float Angle);
		virtual void RotateViaSide      (_float Angle);
		virtual void		 Translate          (Vertex3d ver);
		virtual void		 TranslateViaSide(_float value);
		virtual void		 TranslateViaUp(_float value);
		virtual void		 TranslateViaDirection(_float value);
		virtual void 		 ScaleUniform(_float value) ;
		virtual void 		 ScaleUniformAdd(_float value);
		virtual void 		 Scale(Vertex3d scale);
		

		_float* getTransMtx();
		Vertex4d getQuaternion();
		void     setQuaternion(Vertex4d);


	protected:
		void UpdateTransMtx();

		_u32b						m_ID ;
		_s8b*						m_Name;
		Vertex3d 					m_Position;
		Vertex3d					m_Direction;
		Vertex3d					m_Up;
		Vertex3d					m_Scale ;
		_float*						m_TransMtx;/// 4*4 matrix


	};

};
#endif
