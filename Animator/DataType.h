#ifndef _RGP_ANIMATOR_DATATYPE_H_
#define _RGP_ANIMATOR_DATATYPE_H_

#include "..//Core//Common//BasePrimitiveTypes.h"
#include "..//Core//Common//Math.h"

struct Bone {
	_u8b*			Name;
	_u32b			MeshID;
	_u32b			NumIndices;
	_u32b*			Indices;// index of each affected vertex
	_u32b*			Factor;
	_u32b			ParentIndex;
	_u32b			NumChildren;
	_u32b*			ChildrenIndices;

	Vertex3d		LocalPostion;
	Vertex3d		LocalDirection;
	Vertex3d		LocalUp;

};
typedef struct Bone Bone;
typedef struct Bone Joint;

typedef struct {
	_u32b	NumParts;		//Num bones or joints
	Bone*	Parts;			//Bones/ joints data
} Skeleton;

typedef struct {
	//BoneName
	_u8b*			Name;
	//transformation keys(translation , rotation , Scaling)
	Vertex3d	Tanslation;
	Vertex3d	Rotation;
	Vertex3d	Scaling;
} Key ;
typedef struct {
	_u32b		Instance;
	_u32b		numKeys;
	Key*		Keys;
	_u32b		BlendFunc;///blend function code

} Pose;

typedef struct {
	_u8b	Name;
	_u32b	NumPoses;
	Pose*	Poses;
} Animation;

#endif
