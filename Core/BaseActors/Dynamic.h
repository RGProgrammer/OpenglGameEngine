#ifndef _RGP_DYNAMIC_TYPE_H_
#define _RGP_DYNAMIC_TYPE_H_

#include ".//BaseActor.h"

// Dynamic Actor type ID
#define DYNAMIC              0x00000002

namespace RGP_CORE{
///Dynamic Type
	class Dynamic: public virtual BaseActor {
	protected:
		Dynamic(const _s8b* name = "Actor");
		Dynamic(Vertex3d Pos,const _s8b* name = "Actor");
		Dynamic(Vertex3d Pos, Vertex3d Dir, Vertex3d Up,const _s8b* name = "Actor");
	public:
		virtual ~Dynamic();
		virtual void Update(_float dt)=0;
	};
};
#endif
